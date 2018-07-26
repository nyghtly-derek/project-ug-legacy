#include "Wall.h"

Wall::Wall() : Structure()
{
    this->setID(WALL);
    this->hitPoints = 100;
    this->maxHitPoints = 100;
}

Wall::Wall(int x, int y, int team) : Structure(x, y, team)
{
    this->setID(WALL);
    this->hitPoints = 100;
    this->maxHitPoints = 100;
}

Wall::~Wall()
{
    //dtor
}
