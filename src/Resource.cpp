#include "Resource.h"

Resource::Resource() : MapObject()
{
    this->setID(STONE);
    this->remaining = 500;
    this->setYield(10);
    this->noneLeft = false;
}

Resource::Resource(int x, int y) : MapObject(x, y)
{
    this->setID(STONE);
    this->remaining = 500;
    this->setYield(10);
    this->noneLeft = false;
}

int Resource::exploit()
{
    if (!noneLeft)
    {
        this->remaining -= this->yield;
        if (this->remaining < 0)
        {
            this->noneLeft = true;
            return (this->yield - abs(this->remaining));
        }
        return this->yield;
    }
    return 0;
}

/*******************
 * PROTECTED METHODS
 ********************/

void Resource::setYield(int yield)
{
    this->yield = yield;
}

void Resource::setRemaining(int amount)
{
    this->remaining = amount;
}

Resource::~Resource()
{

}
