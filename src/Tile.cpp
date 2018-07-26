#include "Tile.h"

Tile::Tile()
{
    this->setX(0);
    this->setY(0);
    this->setID(STONE);
    this->flagUnit(false);
}

Tile::Tile(int x, int y, int tileID)
{
    this->setX(x);
    this->setY(y);
    this->setID(tileID);
    this->flagUnit(false);
}

void Tile::flagWall(bool isWall)
{
    this->isWall = isWall;
}

bool Tile::checkWall()
{
    return this->isWall;
}

void Tile::flagUnit(bool hasUnit)
{
    this->hasUnit = hasUnit;
}

bool Tile::checkUnit()
{
    return this->hasUnit;
}

void Tile::setID(int tileID)
{
    this->objectID = tileID;
    if (tileID == STONE)
    {
        this->flagWall(true);
    }
    else
    {
        this->flagWall(false);
    }
}

Tile::~Tile()
{
    //dtor
}
