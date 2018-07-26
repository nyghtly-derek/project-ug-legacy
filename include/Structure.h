#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "MapObject.h"

class Structure : public MapObject
{
    public:
        Structure();
        Structure(int x, int y, int team);
        void receiveAttack(int damage);
        bool checkDestroyed();
        virtual ~Structure();
    protected:
        int hitPoints;
        int maxHitPoints;
    private:
        int team;
        bool crumbling;
        bool destroyed;
};

#endif // STRUCTURE_H
