#include "Goblin.h"

Goblin::Goblin(int x, int y, int team)
    : Unit(x, y, team)
{
    this->name = "Goblin";
    this->setID(WORKER);
    this->moveSpeed = 150;
    this->maxHealth = 10;
    this->health = 10;
    this->attackDmg = 1;
    this->attackSpeed = 1500;
}

Goblin::~Goblin() { }
