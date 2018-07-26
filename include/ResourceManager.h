#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Stone.h"

class ResourceManager
{
    public:
        ResourceManager();
        void add(int x, int y, int tileID);
        bool checkResource(int x, int y);
        int exploit(int x, int y);
        void blitAll(Blitter *blitter);
        virtual ~ResourceManager();
    protected:
    private:
        std::map<xypair, Resource, pntCmpFn> resources;
};

#endif // RESOURCEMANAGER_H
