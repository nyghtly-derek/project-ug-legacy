#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "Additional.h"
#include "Blitter.h"

/**
 * Class: MapObject
 * ----------------
 * This class acts as a single object that can be placed on a game map.
 * This is meant to be inherited by other classes and not used directly.
 *
 * Methods: setX, setY, getX, getY, setID, blit
 */

class MapObject
{
    public:
        MapObject();
        MapObject(int x, int y);

/**
 * Method: setX
 * Usage in Unit: this->setX(next.x);
 * ----------------------------------
 * Sets x value.
 */

        void setX(int x);

/**
 * Method: setY
 * Usage in Unit: this->setY(next.y);
 * ----------------------------------
 * Sets y value.
 */

        void setY(int y);

/**
 * Method: getX
 * Usage in UnitManager:
 *  xypair dyingUnit = {target->getX(), target->getY()};
 * -----------------------------------------------------
 * Returns x value.
 */

        int getX();

/**
 * Method: getY
 * Usage in UnitManager:
 *  xypair dyingUnit = {target->getX(), target->getY()};
 * -----------------------------------------------------
 * Returns y value.
 */

        int getY();

/**
 * Method: setID
 * Usage in Unit: this->setID(WORKER);
 * -----------------------------------------------------
 * Sets object ID, which is used to determine its sprite.
 */


        void setID(int objectID);

/**
 * Method: blit
 * Usage: mapObject->blit(blitter);
 * -----------------------------------------------------
 * Sets object ID, which is used to determine its sprite.
 */

        void blit(Blitter *blitter);

        virtual ~MapObject();

    protected:
        int objectID;

    private:
        int x;
        int y;
};

#endif // MAPOBJECT_H
