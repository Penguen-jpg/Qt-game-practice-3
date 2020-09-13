#include "HexBoard.h"
#include "Game.h"
extern Game *game;

HexBoard::HexBoard()
{

}

QList<Hex *> HexBoard::getHexes()
{
    return hexes;
}

void HexBoard::placeHexes(int x, int y, int cols, int rows)
{
    int X_SHIFT=82;//x的位移
    int Y_SHIFT=41;//y的位移
    for(int n=0;n<cols;n++)
    {
        //偶數列不動，基數列向下移動
        if(n%2==0)
        {
            Y_SHIFT=0;
        }else
        {
            Y_SHIFT=41;
        }
        createHexColumn(x+X_SHIFT*n,y+Y_SHIFT,rows);
    }
}

void HexBoard::createHexColumn(int x, int y, int numOfRows)
{
    for(int n=0;n<numOfRows;n++)
    {
        Hex *hex=new Hex();
        hex->setPos(x,y+82*n);//放置hex
        hexes.append(hex);//儲存hex
        game->scene->addItem(hex);//將hex加入場景

        //將未放置的hex初始化
        hex->setOwner("NONE");
        hex->setIsPlaced(true);
    }
}
