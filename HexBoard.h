#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <QList>
#include "Hex.h"
class HexBoard
{
    public:
        HexBoard();

        //getter/setter
        QList<Hex*> getHexes();

        void placeHexes(int x,int y,int cols,int rows);//放置hexes
    private:
        void createHexColumn(int x,int y,int numOfRows);//在特定位置建造一行的hexes

        QList<Hex*> hexes;//存放目前的hexes
};

#endif // HEXBOARD_H
