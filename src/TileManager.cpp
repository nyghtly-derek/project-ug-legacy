#include "TileManager.h"

TileManager::TileManager()
{
    //ctor
}

void TileManager::add(int x, int y, int tileID)
{
    this->tiles[{x,y}] = Tile(x, y, tileID);
}

void TileManager::flagUnit(int x, int y, bool hasUnit)
{
    this->tiles[{x,y}].flagUnit(hasUnit);
}

bool TileManager::checkUnit(int x, int y)
{
    return this->tiles[{x,y}].checkUnit();
}

void TileManager::flagWall(int x, int y, bool isWall)
{
    this->tiles[{x,y}].flagWall(isWall);
}

bool TileManager::checkWall(int x, int y)
{
    return this->tiles[{x,y}].checkWall();
}

bool TileManager::checkPathable(int x, int y, bool checkUnits)
{
    if (!this->tiles[{x,y}].checkWall())
    {
        if (checkUnits)
        {
            if (!this->tiles[{x,y}].checkUnit())
            {
                 return true;
            }
            else
            {
                #ifdef PRINT_INFO
                std::cout << "TileManager::checkPathable -> Unit on tile." << std::endl;
                #endif // PRINT_INFO
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}

void TileManager::blitAll(Blitter *blitter)
{
    for (std::map<xypair, Tile, pntCmpFn>::iterator it = this->tiles.begin();
            it != this->tiles.end(); it++)
    {
        it->second.blit(blitter);
    }
}

TileManager::~TileManager()
{
    //dtor
}
