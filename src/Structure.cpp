#include "Structure.h"

Structure::Structure() : MapObject()
{
    this->setID(WALL);
    this->crumbling = false;
    this->destroyed = false;
    this->team = 0;
}

Structure::Structure(int x, int y, int team) : MapObject(x, y)
{
    this->setID(WALL);
    this->crumbling = false;
    this->destroyed = false;
    this->team = team;
}

void Structure::receiveAttack(int damage)
{
    if (!this->destroyed)
    {
        this->hitPoints -= damage;
        if (this->hitPoints <= 0)
        {
            this->destroyed = true;
            this->crumbling = false;
            return;
        }
        else if (this->hitPoints <= (this->maxHitPoints / 2))
        {
            this->crumbling = true;
            return;
        }
    }
}

bool Structure::checkDestroyed()
{
    return this->destroyed;
}

Structure::~Structure()
{
    //dtor
}
