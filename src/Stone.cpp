#include "Stone.h"

Stone::Stone() : Resource()
{
    this->setID(STONE);
    this->setRemaining(2000);
    this->setYield(100);
}

Stone::Stone(int x, int y) : Resource(x, y)
{
    this->setID(STONE);
    this->setRemaining(2000);
    this->setYield(100);
}

Stone::~Stone()
{
}
