#include "Additional.h"

float distBtwnPnts(xypair one, xypair two)
{
    return sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2));
}

float distBtwnPnts(node one, node two)
{
    return sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2));
}

bool testAdjacency(xypair a, xypair b)
{
    int diffX = abs(a.x - b.x);
    int diffY = abs(a.y - b.y);
    if (diffX == 0) { diffX++; }
    if (diffY == 0) { diffY++; }
    if (diffX == 1 && diffY == 1)
    {
        return true;
    }
    return false;
}

void clearNodeStack(std::stack<node> *nodeStack)
{
    while (!nodeStack->empty())
    {
        nodeStack->pop();
    }
}

xypair getMinPair(xypair one, xypair two)
{
    xypair minPair;
    minPair.x = one.x;
    if (one.x > two.x)
    {
        minPair.x = two.x;
    }
    minPair.y = one.y;
    if (one.y > two.y)
    {
        minPair.y = two.y;
    }
    return minPair;
}

xypair getMaxPair(xypair one, xypair two)
{
    xypair maxPair;
    maxPair.x = one.x;
    if (one.x < two.x)
    {
        maxPair.x = two.x;
    }
    maxPair.y = one.y;
    if (one.y < two.y)
    {
        maxPair.y = two.y;
    }
    return maxPair;
}

