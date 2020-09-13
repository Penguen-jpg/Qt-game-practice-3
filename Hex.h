#ifndef HEX_H
#define HEX_H
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
class Hex:public QGraphicsPolygonItem
{
    public:
        Hex(QGraphicsItem *parent=NULL);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);//按滑鼠的事件
        void displaySideAttacks();//顯示攻擊力
        void createLines();//建立線以偵測附近的hex
        void findNeighbors();//找出附近的hex
        void switchOwner();//替換持有者
        void captureNeighbors();//捕獲附近的hex

        //getter
        int getAttackOf(int side);
        bool getIsPlaced();
        QString getOwner();

        //setter
        void setAttackOf(int side,int attack);
        void setIsPlaced(bool b);
        void setOwner(QString player);
    private:
        QString owner;//持有者
        bool isPlaced;//是否已放置
        //每個邊的攻擊力
        int side0Attack;
        int side1Attack;
        int side2Attack;
        int side3Attack;
        int side4Attack;
        int side5Attack;
        QList<QGraphicsTextItem*> attackTexts;//儲存每邊的攻擊力
        QList<Hex*> neighbors;//儲存附近的hex
        QList<QGraphicsLineItem*> lines;//儲存hex上的線
};

#endif // HEX_H
