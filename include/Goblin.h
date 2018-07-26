
#ifndef GOBLIN_H
#define GOBLIN_H

#include "Unit.h"

/**
 * Subclass: Goblin
 * ----------------
 * As a subclass of Unit, Goblin has a custom constructor that
 * loads the appropriate stats for a WORKER unit. After further
 * subclassing and unit additions, this class will be more
 * general.
 */

class Goblin : public Unit
{
    public:
        Goblin(int x, int y, int team);
        virtual ~Goblin();
    protected:
    private:
};

#endif // GOBLIN_H
