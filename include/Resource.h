#ifndef RESOURCE_H
#define RESOURCE_H

#include "MapObject.h"

class Resource : public MapObject
{
    public:
        Resource();
        Resource(int x, int y);
        int exploit();
        virtual ~Resource();
    protected:
        void setYield(int yield);
        void setRemaining(int amount);
    private:
        int yield;
        int remaining;
        bool noneLeft;
};

#endif // RESOURCE_H
