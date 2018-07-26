
#ifndef HUMAN_H
#define HUMAN_H

#include "Unit.h"

/**
 * Subclass: Human
 * ---------------
 * As a subclass of Unit, Human has a custom constructor that
 * loads the appropriate stats for a KNIGHT unit. After further
 * subclassing and unit additions, this class will be more
 * general.
 */

class Human : public Unit
{
    public:
        Human(int x, int y, int team);
        virtual ~Human();
    protected:
    private:
};

#endif // HUMAN_H
