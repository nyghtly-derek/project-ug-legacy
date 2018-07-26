#ifndef STONE_H
#define STONE_H

#include <Resource.h>


class Stone : public Resource
{
    public:
        Stone();
        Stone(int x, int y);
        virtual ~Stone();
    protected:
    private:
};

#endif // STONE_H
