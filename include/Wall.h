#ifndef WALL_H
#define WALL_H

#include "Structure.h"


class Wall : public Structure
{
    public:
        Wall();
        Wall(int x, int y, int team);
        virtual ~Wall();
    protected:
    private:
};

#endif // WALL_H
