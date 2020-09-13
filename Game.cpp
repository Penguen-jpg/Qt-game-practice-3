#include "Game.h"
#include "Hex.h"
#include "Button.h"
#include <QFont>
#include <stdlib.h>
#include <time.h>

Game::Game(QWidget *parent)
{
    //設定view
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1024,768);

    //設定scene
    scene=new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setScene(scene);

    //初始化
    cardToPlace=NULL;
    numCardsPlaced=0;

    //產生亂數種子
    srand(time(NULL));
}

void Game::start()
{  
    //初始化(重新遊玩時會用到)
    cardToPlace=NULL;
    numCardsPlaced=0;

    scene->clear();//清畫面

    hexBoard=new HexBoard();
    hexBoard->placeHexes(200,30,3,3);

    drawGUI();//畫出介面
    createInitialCards();//初始化玩家牌組
}

void Game::restartGame()
{
    //清除所有東西再開始
    player1Cards.clear();
    player2Cards.clear();
    hexBoard->getHexes().clear();
    scene->clear();
    start();
}

void Game::displayMainMenu()
{
    //設定標題
    QGraphicsTextItem *title=new QGraphicsTextItem("Hex War");
    QFont titleFont("comic sans",50);
    title->setFont(titleFont);
    int txPos=this->width()/2-title->boundingRect().width()/2;
    int tyPos=150;
    title->setPos(txPos,tyPos);
    scene->addItem(title);

    //設定開始按鈕
    Button *playButton=new Button("Play");
    int pxPos=this->width()/2-playButton->boundingRect().width()/2;
    int pyPos=275;
    playButton->setPos(pxPos,pyPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));//連接clicked和start
    scene->addItem(playButton);

    //設定離開按鈕
    Button *quitButton=new Button("Quit");
    int qxPos=this->width()/2-quitButton->boundingRect().width()/2;
    int qyPos=350;
    quitButton->setPos(qxPos,qyPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));//連接clicked和start
    scene->addItem(quitButton);
}

void Game::createNewCard(QString player)
{
    //初始化新牌
    Hex *card=new Hex();
    card->setOwner(player);
    card->setIsPlaced(false);

    //為每邊決定攻擊力
    for(int n=0;n<6;n++)
    {
        int random=rand()%6+1;//攻擊力範圍在1~7
        card->setAttackOf(n,random);
    }

    //顯示攻擊力
    card->displaySideAttacks();

    //將牌組依照持有者放入QList
    if(player=="PLAYER1")
    {
        player1Cards.append(card);
    }else if(player=="PLAYER2")
    {
        player2Cards.append(card);
    }

    //畫出牌組
    drawCards();
}

void Game::createInitialCards()
{
    //幫玩家1和玩家2建立初始hex
    for(int n=0;n<5;n++)
    {
        createNewCard("PLAYER1");
    }
    for(int n=0;n<5;n++)
    {
        createNewCard("PLAYER2");
    }
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    //如果cardToPlace有指向某個hex，則讓他跟著滑鼠移動
    if(cardToPlace)
    {
        cardToPlace->setPos(event->pos());
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void Game::pickUpCard(Hex *card)
{
    //拿起某張牌
    if(card->getOwner()==getWhoseTurn()&cardToPlace==NULL)
    {
        cardToPlace=card;
        originalPos=card->pos();
        return;
    }
}

void Game::placeCard(Hex *hexToReplace)
{
    //將cardToPlace替換到這個hex
    cardToPlace->setPos(hexToReplace->pos());
    hexBoard->getHexes().removeAll(hexToReplace);//將hexToReplace從QList中移除
    hexBoard->getHexes().append(cardToPlace);//將cardToPlace加入QList中
    scene->removeItem(hexToReplace);//將hexToReplace從場景中移除
    cardToPlace->setIsPlaced(true);//將cardToPlace設定為已放置
    removeFromDeck(cardToPlace,getWhoseTurn());//將cardToPlace從牌組中移除

    //找出鄰居，並試著捕獲
    cardToPlace->findNeighbors();
    cardToPlace->captureNeighbors();

    //初始化cardToPlace
    cardToPlace=NULL;

    //補上新牌
    createNewCard(getWhoseTurn());

    //輪到下個玩家的回合
    nextPlayersTurn();

    //更新資訊
    numCardsPlaced++;

    //如果棋盤滿了，則遊戲結束
    if(numCardsPlaced>=hexBoard->getHexes().size())
    {
        gameOver();
    }
}

void Game::nextPlayersTurn()
{
    if(getWhoseTurn()=="PLAYER1")
    {
        setWhoseTurn("PLAYER2");
    }else
    {
       setWhoseTurn("PLAYER1");
    }
}

void Game::removeFromDeck(Hex *card, QString player)
{
    //依據玩家是誰來移除卡牌
    if(player=="PLAYER1")
    {
        player1Cards.removeAll(card);
    }else
    {
        player2Cards.removeAll(card);
    }
}

void Game::mousePressEvent(QMouseEvent *event)
{
    //如果按了右鍵，則取消放置
    if(event->button()==Qt::RightButton)
    {
        if(cardToPlace)
        {
            cardToPlace->setPos(originalPos);
            cardToPlace=NULL;
            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void Game::gameOver()
{
    //兩位玩家在棋盤上的hex數量
    int p1hexes=0;
    int p2hexes=0;

    //數出每位玩家的hex數量
    for(int n=0;n<hexBoard->getHexes().size();n++)
    {
        if(hexBoard->getHexes()[n]->getOwner()=="PLAYER1")
        {
            p1hexes++;
        }else if(hexBoard->getHexes()[n]->getOwner()=="PLAYER2")
        {
            p2hexes++;
        }
    }

    QString message;//結束畫面所顯示的字串
    if(p1hexes>p2hexes)
    {
        message="Player 1 has won!";
    }else if(p2hexes>p1hexes)
    {
        message="Player 2 has won";
    }else
    {
        message="Tie game!";
    }

    displayGameOverWindow(message);
}

void Game::displayGameOverWindow(QString textToDisplay)
{
    //將場景內的所有物件停用
    for(int n=0;n<scene->items().size();n++)
    {
        scene->items()[n]->setEnabled(false);
    }

    //跳出半透明的面板
    drawPanel(0,0,1024,768,Qt::black,0.65);
    drawPanel(312,184,400,400,Qt::lightGray,0.75);

    //建立重新遊玩按鈕
    Button *playAgain=new Button("Play again");
    playAgain->setPos(410,300);
    scene->addItem(playAgain);
    connect(playAgain,SIGNAL(clicked()),this,SLOT(restartGame()));

    //建立退出按鈕
    Button *quit=new Button("Quit");
    quit->setPos(410,375);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));

    //顯示的字串
    QGraphicsTextItem *overText=new QGraphicsTextItem(textToDisplay);
    overText->setPos(460,225);
    scene->addItem(overText);
}

QString Game::getWhoseTurn()
{
    return whoseTurn;
}

void Game::setWhoseTurn(QString player)
{
    //更新資訊
    whoseTurn=player;
    whoseTurnText->setPlainText(QString("Whose turn:")+player);
}

void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity)
{
    //畫出面板
    QGraphicsRectItem *panel=new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);//設定不透明度
    scene->addItem(panel);//將面板加入場景
}

void Game::drawGUI()
{
    //畫出左面板
    drawPanel(0,0,150,768,Qt::darkCyan,0.5);

    //畫出右面板
    drawPanel(874,0,150,768,Qt::darkCyan,0.5);

    //放置玩家1text
    QGraphicsTextItem *p1=new QGraphicsTextItem("Player1's cards:");
    p1->setPos(25,0);
    scene->addItem(p1);

    //放置玩家2text
    QGraphicsTextItem *p2=new QGraphicsTextItem("Player2's cards:");
    p2->setPos(874+25,0);
    scene->addItem(p2);

    //放置回合資訊
    whoseTurnText=new QGraphicsTextItem();
    setWhoseTurn(QString("PLAYER1"));
    whoseTurnText->setPos(490,0);
    scene->addItem(whoseTurnText);
}

void Game::drawCards()//將QList內的每張牌加入場景
{
    //從場景移除玩家所有牌組
    for(int n=0;n<player1Cards.size();n++)
    {
        scene->removeItem(player1Cards[n]);
    }
    for(int n=0;n<player2Cards.size();n++)
    {
        scene->removeItem(player2Cards[n]);
    }

    //重新加入牌組至場景內
    for(int n=0;n<player1Cards.size();n++)
    {
        Hex *card=player1Cards[n];
        card->setPos(13,25+85*n);
        scene->addItem(card);
    }
    for(int n=0;n<player2Cards.size();n++)
    {
        Hex *card=player2Cards[n];
        card->setPos(874+13,25+85*n);
        scene->addItem(card);
    }
}
