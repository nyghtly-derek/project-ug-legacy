#include "StructureManager.h"

StructureManager::StructureManager()
{
    //ctor
}

void StructureManager::add(int x, int y, int team, int structureID)
{
    #ifdef PRINT_INFO
    std::cout << "StructureManager::add -> adding new structure at " << "(" << x << "," << y << ")" << std::endl;
    #endif // PRINT_INFO
    if (structureID == WALL)
    {
        this->structures[{x,y}] = Wall(x, y, team);
    }
    else
    {
        // when more are added then this won't be redundant
        this->structures[{x, y}] = Wall(x, y, team);
    }
}

bool StructureManager::checkPathable(int x, int y)
{
    if (this->structures.find({x, y}) == this->structures.end())
    {
        return true;
    }
    if (this->structures[{x,y}].checkDestroyed())
    {
        return true;
    }
    return false;
}

void StructureManager::blitAll(Blitter *blitter)
{
    for (std::map<xypair, Structure, pntCmpFn>::iterator it = this->structures.begin();
            it != this->structures.end(); it++)
    {
        it->second.blit(blitter);
    }
}

StructureManager::~StructureManager()
{
    //dtor
}
