#include "MapObject.h"

MapObject::MapObject()
{
    this->setX(0);
    this->setY(0);
    this->setID(STONE);
}

MapObject::MapObject(int x, int y)
{
    this->setX(x);
    this->setY(y);
    this->setID(STONE);
}

void MapObject::setX(int x)
{
    this->x = x;
}

void MapObject::setY(int y)
{
    this->y = y;
}

int MapObject::getX()
{
    return this->x;
}

int MapObject::getY()
{
    return this->y;
}

void MapObject::setID(int objectID)
{
    this->objectID = objectID;
}

void MapObject::blit(Blitter *blitter)
{
    blitter->blitSprite(this->objectID, this->x * 32, this->y * 32, true);
}

MapObject::~MapObject()
{
    //dtor
}
