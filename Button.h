#ifndef BUTTON_H
#define BUTTON_H
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
class Button:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
    public:
        Button(QString name,QGraphicsItem *parent=NULL);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);//滑鼠按按鈕的事件
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);//滑鼠在按鈕上的事件
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);//滑鼠離開按鈕的事件
    signals:
        void clicked();
    private:
        QGraphicsTextItem *text;
};

#endif // BUTTON_H
