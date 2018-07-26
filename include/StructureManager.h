#ifndef STRUCTUREMANAGER_H
#define STRUCTUREMANAGER_H

#include "Wall.h"

class StructureManager
{
    public:
        StructureManager();
        void add(int x, int y, int team, int structureID);
        bool checkPathable(int x, int y);
        void blitAll(Blitter *blitter);
        virtual ~StructureManager();
    protected:
    private:
        std::map<xypair, Structure, pntCmpFn> structures;
};

#endif // STRUCTUREMANAGER_H
