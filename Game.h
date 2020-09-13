#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include "HexBoard.h"
#include <QList>
#include <QMouseEvent>
/*1.修正了點擊灰格導致程式終止
 *2.修正有時出現無法捕獲鄰居的情況
*/
class Game:public QGraphicsView
{
    Q_OBJECT
    public:
        Game(QWidget *parent=NULL);

        void displayMainMenu();//顯示主選單
        void mouseMoveEvent(QMouseEvent *event);//滑鼠移動的事件
        void pickUpCard(Hex *card);//拿起牌
        void placeCard(Hex *hexToReplace);//放置牌
        void nextPlayersTurn();//輪到下個玩家的回合
        void removeFromDeck(Hex *card,QString player);//將某張牌從牌組中移除
        void mousePressEvent(QMouseEvent *event);//按滑鼠的事件
        void gameOver();//遊戲結束
        void displayGameOverWindow(QString textToDisplay);//遊戲結束時顯示的視窗

        //getter
        QString getWhoseTurn();

        //setter
        void setWhoseTurn(QString player);

        QGraphicsScene *scene;//場景
        HexBoard *hexBoard;//棋板
        Hex *cardToPlace;//指向目前要放置的hex
        QPointF originalPos;//某張牌原本的位置
        int numCardsPlaced;//放置的hex數量

    public slots:
        void start();//開始遊戲
        void restartGame();//重新遊玩
    private:
        void drawPanel(int x,int y,int width,int height,QColor color,double opactiy);//畫出面板
        void drawGUI();//畫出使用者介面
        void drawCards();//畫出牌組
        void createNewCard(QString player);//建立新牌
        void createInitialCards();//建立初始的牌組

        QString whoseTurn;//誰的回合
        QGraphicsTextItem *whoseTurnText;//回合資訊
        QList<Hex*> player1Cards;//玩家1牌組
        QList<Hex*> player2Cards;//玩家2牌組
};

#endif // GAME_H
