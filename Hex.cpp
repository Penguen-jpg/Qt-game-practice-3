#include "Hex.h"
#include <QVector>
#include <QPointF>
#include <QBrush>
#include "Game.h"
#include <QDebug>
extern Game *game;

Hex::Hex(QGraphicsItem *parent)
{
    QVector<QPointF> hexPoints;//儲存hex的點
    hexPoints <<QPointF(1,0)<<QPointF(2,0)<<QPointF(3,1)<<QPointF(2,2)//輸入點
              << QPointF(1,2)<<QPointF(0,1);

    //放大每個點
    int SCALE_BY=40;
    for(int n=0;n<hexPoints.size();n++)
    {
        hexPoints[n]*=SCALE_BY;
    }

    //建立多邊形並畫出
    QPolygonF hexgon(hexPoints);
    setPolygon(hexgon);

    //初始化
    isPlaced=false;
    side0Attack=0;
    side1Attack=0;
    side2Attack=0;
    side3Attack=0;
    side4Attack=0;
    side5Attack=0;

    //建立線
    createLines();

    //建立QGraphicsTextItem來顯示攻擊力
    QGraphicsTextItem *text0=new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem *text1=new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem *text2=new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem *text3=new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem *text4=new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem *text5=new QGraphicsTextItem(QString::number(0),this);

    //存入每邊攻擊力
    attackTexts.append(text0);
    attackTexts.append(text1);
    attackTexts.append(text2);
    attackTexts.append(text3);
    attackTexts.append(text4);
    attackTexts.append(text5);

    //設定攻擊力出現的位置
    text0->setPos(50,0);
    text1->setPos(20,15);
    text2->setPos(20,40);
    text3->setPos(50,55);
    text4->setPos(80,40);
    text5->setPos(80,15);

    //隱藏所有攻擊力
    for(int n=0;n<attackTexts.size();n++)
    {
        attackTexts[n]->setVisible(false);
    }
}

void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //如果這個hex還沒被放置，則將他拿起;如果已被放置，則替換他
    if(getIsPlaced()==false)
    {
        game->pickUpCard(this);
    }else
    {
        game->placeCard(this);
    }
}

void Hex::displaySideAttacks()
{
    //顯示所有攻擊力
    for(int n=0;n<attackTexts.size();n++)
    {
        attackTexts[n]->setVisible(true);
    }
}

void Hex::createLines()
{
    QPointF hexCenter(x()+60,y()+40);//hex的中心
    QPointF finalPoint(hexCenter.x(),hexCenter.y()-65);//線的端點
    QLineF ln(hexCenter,finalPoint);//建立中心到端點的線

    for(int n=0;n<6;n++)
    {
        QLineF lnCopy(ln);//line的複製品
        lnCopy.setAngle(90+60*n);//將線旋轉以配合角度
        QGraphicsLineItem *line=new QGraphicsLineItem(lnCopy,this);//用lnCopy建立線物件
        lines.append(line);//存入線
        line->setVisible(false);//隱藏
    }
}

void Hex::findNeighbors()
{
    for(int n=0;n<lines.size();n++)//找出所有線所碰到的hex
    {
        QList<QGraphicsItem*> cItems=lines[n]->collidingItems();//儲存碰到這條線的物件
        for(int k=0;k<cItems.size();k++)
        {
            Hex *item=dynamic_cast<Hex*>(cItems[k]);
            if(cItems[k]!=this&&item)//如果碰到的物件不是自己且item可轉型成hex，則代表這個hex是鄰居
            {
                neighbors.append(item);
                touch.append(n);//紀錄第幾個邊碰到
            }
        }
    }
}

void Hex::switchOwner()
{
    if(getOwner()=="PLAYER1")
    {
        setOwner("PLAYER2");
    }else if(getOwner()=="PLAYER2")
    {
        setOwner("PLAYER1");
    }
}

void Hex::captureNeighbors()
{
    //走訪所有鄰居，捕獲比自己還弱的鄰居
    for(int n=0;n<neighbors.size();n++)
    {
        bool isEnemy=false;//是否為敵人
        bool isNotNeutral=false;//是否沒有持有者

        if(this->getOwner()!=neighbors[n]->getOwner())
        {
            isEnemy=true;
        }
        if(neighbors[n]->getOwner()!="NONE")
        {
            isNotNeutral=true;
        }

        //如果確定是敵人且有持有者，則判斷是否能捕獲
        if(isEnemy&&isNotNeutral)
        {
            int thisAttack=0;//自己的攻擊力
            int neighborsAttack=0;//鄰居的攻擊力

            if (n==0){
                thisAttack=getAttackOf(0);
                neighborsAttack=neighbors[0]->getAttackOf(3);
            }
            else if (n==1){
                thisAttack=getAttackOf(1);
                neighborsAttack=neighbors[1]->getAttackOf(4);
            }
            else if (n==2){
                thisAttack=getAttackOf(2);
                neighborsAttack=neighbors[2]->getAttackOf(5);
            }
            else if (n==3){
                thisAttack=getAttackOf(3);
                neighborsAttack=neighbors[3]->getAttackOf(0);
            }
            else if (n==4){
                thisAttack=getAttackOf(4);
                neighborsAttack=neighbors[4]->getAttackOf(1);
            }
            else if (n==5){
                thisAttack=getAttackOf(5);
                neighborsAttack=neighbors[5]->getAttackOf(2);
            }

            //如果攻擊力比鄰居大，則捕獲(將他的持有者變成自己)
            if (thisAttack > neighborsAttack){
                neighbors[n]->switchOwner();
            }
        }
    }
}

int Hex::getAttackOf(int side)
{
    if(side==0)
    {
        return side0Attack;
    }else if(side==1)
    {
        return side1Attack;
    }else if(side==2)
    {
        return side2Attack;
    }else if(side==3)
    {
        return side3Attack;
    }else if(side==4)
    {
        return side4Attack;
    }else if(side==5)
    {
        return side5Attack;
    }
}

bool Hex::getIsPlaced()
{
    return isPlaced;
}

QString Hex::getOwner()
{
    return owner;
}

void Hex::setAttackOf(int side, int attack)
{
    switch (side)
    {
        case 0:
            side0Attack=attack;
            attackTexts[0]->setPlainText(QString::number(attack));
        case 1:
            side1Attack=attack;
            attackTexts[1]->setPlainText(QString::number(attack));
        case 2:
            side2Attack=attack;
            attackTexts[2]->setPlainText(QString::number(attack));
        case 3:
            side3Attack=attack;
            attackTexts[3]->setPlainText(QString::number(attack));
        case 4:
            side4Attack=attack;
            attackTexts[4]->setPlainText(QString::number(attack));
        case 5:
            side5Attack=attack;
            attackTexts[5]->setPlainText(QString::number(attack));
    }
}

void Hex::setIsPlaced(bool b)
{
    isPlaced=b;
}

void Hex::setOwner(QString player)
{
    //更新資訊
    owner=player;

    //依據持有者不同來改變顏色
    if(player=="NONE")
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGray);
        setBrush(brush);
    }
    if(player=="PLAYER1")
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::blue);
        setBrush(brush);
    }
    if(player=="PLAYER2")
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        setBrush(brush);
    }
}
