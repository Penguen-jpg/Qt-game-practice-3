#include "Button.h"
#include <QBrush>
Button::Button(QString name, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    //設定按鈕大小
    setRect(0,0,200,50);

    //設定brush
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    setBrush(brush);

    //畫出text
    text=new QGraphicsTextItem(name,this);
    //將text放到按鈕正中間
    int xPos=rect().width()/2-text->boundingRect().width()/2;
    int yPos=rect().height()/2-text->boundingRect().height()/2;
    text->setPos(xPos,yPos);

    //設定成可接受hoverEvent
    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();//啟動clicked() signal
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //將按鈕顏色變成cyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::cyan);
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //將按鈕顏色變成darkcyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    setBrush(brush);
}
