
#ifndef ADDITIONAL_H
#define ADDITIONAL_H
//#define PRINT_INFO
//#define DRAW_INFO

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stack>
#include <vector>
#include <set>
#include <memory>
#include <map>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <string>

#include <stdlib.h>
#include <cstdlib>
#include <cmath>

/** IDs for sprite source locations on bitmap */
enum spriteTypes {GRASS, STONE, WORKER, CIRCLE, KNIGHT, WALL};

/**
 * Structure: spriteSrcLoc
 * -----------------------
 * Stores the location of a sprite in a bitmap, as well as the
 * dimensions of the sprite.
 */

struct spriteSrcLoc
{
    int x;
    int y;
    int w;
    int h;
};

/**
 * Structure: xypair
 * -----------------
 * Stores x and y coordinates so that return values can have both.
 */

struct xypair
{
    int x;
    int y;
};

/**
 * Structure: node
 * ---------------
 * Stores x and y coordinates, a parent node, as well some
 * heuristics. f is the overall value of the node, which is simply
 * the sum of g and h. g is the distance traveled so far. h is the
 * distance to the goal.
 */

struct node
{
    const node *parent;
    int x, y;
    float moveCost, distTravld, distToDest;
};

/**
 * Structure: pntCmpFn
 * -------------------
 * Defines comparison operator so that xypair can be used with
 * ordered stl classes.
 */

struct pntCmpFn
{
    bool operator() (xypair one, xypair two) const
    {
        if ( one.x < two.x )
            return true;
        if ( one.y < two.y && one.x == two.x )
            return true;
        return false;
    }
};

/**
 * Structure: nodeCmpFn
 * --------------------
 * Defines comparison operator so that node can be used with
 * ordered stl classes.
 */

struct nodeCmpFn
{
    bool operator() (node one, node two)
    {
        if ( one.x < two.x )
            return true;
        if ( one.y < two.y && one.x == two.x )
            return true;
        return false;
    }
};

/**
 * Structure: timeKeep
 * ---------------------
 * Used in Timer for converting from milliseconds to hr:min:sec.
 */

struct timeKeep
{
    int hours;
    int minutes;
    int seconds;
};

/**
 * Function: distBtwnPoints
 * ------------------------
 * Returns the distance between the nodes or xypairs that the
 * user provides.
 */

float distBtwnPnts(xypair one, xypair two);
float distBtwnPnts(node one, node two);

/**
 * Function: testAdjacency
 * -----------------------
 * Returns true if a and b are within one point of each other.
 */

bool testAdjacency(xypair a, xypair b);

/**
 * Function: clearNodeStack
 * ------------------------
 * Pops off all nodes from the provided node stack.
 */

void clearNodeStack(std::stack<node> *nodeStack);

/**
 * Function: getMinPair
 * Usage: minPair = getMinPair(cursorCurr, leftHeldDown);
 * ------------------------------------------------------
 * Returns the minimum pair of the provided coordinates.
 */

xypair getMinPair(xypair one, xypair two);

/**
 * Function: getMaxPair
 * Usage: maxPair = getMaxPair(cursorCurr, leftHeldDown);
 * ------------------------------------------------------
 * Returns the maximum pair of the provided coordinates.
 */

xypair getMaxPair(xypair one, xypair two);


#endif // ADDITIONAL_H

