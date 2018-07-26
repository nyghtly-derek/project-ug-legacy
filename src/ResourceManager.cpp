#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
    //ctor
}

void ResourceManager::add(int x, int y, int resourceID)
{
    if (resourceID == STONE)
    {
        this->resources[{x, y}] = Stone(x, y);
    }
    else
    {
        // when more are added then this won't be redundant
        this->resources[{x, y}] = Stone(x, y);
    }
}

bool ResourceManager::checkResource(int x, int y)
{
    if (this->resources.find({x, y}) == this->resources.end())
    {
        return false;
    }
    return true;
}

int ResourceManager::exploit(int x, int y)
{
    this->resources[{x, y}].exploit();
}

void ResourceManager::blitAll(Blitter *blitter)
{
    for (std::map<xypair, Resource, pntCmpFn>::iterator it = this->resources.begin();
            it != this->resources.end(); it++)
    {
        it->second.blit(blitter);
    }
}

ResourceManager::~ResourceManager()
{
    //dtor
}
