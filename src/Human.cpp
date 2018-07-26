#include "Human.h"

Human::Human(int x, int y, int team)
    : Unit(x, y, team)
{
    this->name = "Knight";
    this->setID(KNIGHT);
    this->moveSpeed = 350;
    this->maxHealth = 25;
    this->health = 25;
    this->attackDmg = 2;
    this->attackSpeed = 2000;
}

Human::~Human() { }
