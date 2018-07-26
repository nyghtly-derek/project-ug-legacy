
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "TileManager.h"
#include "UnitManager.h"
#include "ResourceManager.h"
#include "StructureManager.h"
#include "Timer.h"

/**
 * Class: GameMap
 * --------------
 * This class stores information on the units, tiles, and anything
 * else related to the map a match is played on.
 *
 * Methods: loadMap,getWidth, getHeight, flagUnit, checkUnit, checkPathable,
 *          removeUnit, select, deselect, findPath, findTarget, clearMap,
 *          drawMap, updateMap
 */

class GameMap
{
public:
    GameMap();

/**
 * Method: loadMap
 * Usage in GameLoop:
 *  while (gmap->loadMap(MAPNAME, blitter) == false)...
 * -----------------------------------------------------------
 * Loads in a map using information from selected map file. Returns
 * true if map loaded successfully, false if not. Examples of
 * appropriate map files can be found in the data folder. The first
 * line indicates dimension; the first block indicates tiles; and the
 * second block indicates units. If loading fails, prints error
 * message to console.
 */

    bool loadMap(std::string mapFilename, Blitter *blitter);

/**
 * Method: getWidth
 * Usage in GameLoop:
 *  camera->slide(camOffsets.x, camOffsets.y, gmap->getWidth(), gmap->getHeight());
 * --------------------------------------------------------------------------------
 * Returns the width of the game map.
 */

    int getWidth();

/**
 * Method: getWidth
 * Usage in GameLoop:
 *  camera->slide(camOffsets.x, camOffsets.y, gmap->getWidth(), gmap->getHeight());
 * --------------------------------------------------------------------------------
 * Returns the height of the game map.
 */

    int getHeight();

/**
 * Method: flagUnit
 * Usage in UnitManager: gmap->flagUnit(current->getX(), current->getY(), false);
 * ------------------------------------------------------------------------------
 * Flags the unit as present (if hasUnit true) on (x,y) or not present
 * (if hasUnit false).
 */

   void flagUnit(int x, int y, bool hasUnit);

/**
 * Method: checkUnit
 * Usage in GameLoop: if (gmap->checkUnit(held.x, held.y))...
 * ---------------------------------------------------------------
 * Returns true if there is a unit at position (x,y), false if not.
 */

    bool checkUnit(int x, int y);

/**
 * Method: checkPathable
 * Usage in Unit: if (gmap->checkPathable(next.x, next.y, true))...
 * ----------------------------------------------------------------
 * Returns true if tile can be occupied by a unit. If checkUnits is
 * true, then takes units into account.
 */

    bool checkPathable(int x, int y, bool checkUnits);

/**
 * Method: removeUnit
 * Usage in UnitManager: gmap->removeUnit((*it).x, (*it).y);
 * ---------------------------------------------------------
 * Removes unit from the game map.
 */

    void removeUnit(int x, int y);

/**
 * Method: select
 * Usage in GameLoop: gmap->select(toBeSelected[i].x, toBeSelected[i].y);
 * ----------------------------------------------------------------------
 * Selects units from game map, meaning future calls of
 * findPath or findTarget for selected units will use these
 * units.
 */

    void select(int x, int y);

/**
 * Method: deselect
 * Usage in GameLoop: gmap->deselect(-1, -1);
 * ------------------------------------------
 * Deselects unit from game map, meaning it will no longer
 * be used when calling findPath or findTarget for selected
 * units.
 */

    void deselect(int x, int y);

/**
 * Method: findPath
 * Usage in GameLoop: gmap->findPath(-1, -1, mapLoc.x, mapLoc.y);
 * --------------------------------------------------------------
 * Finds a path for target unit from their current location
 * to (destX, destY). Unit will store the path and automatically
 * move when updateMap is called (depending on move speed of unit).
 * Passing in (-1, -1) for (x, y) will call this method for all
 * selected units.
 */

    void findPath(int x, int y, int destX, int destY);


/**
 * Method: findTarget
 * Usage in GameLoop: gmap->findTarget(-1, -1);
 * -------------------------------------------------------------
 * Finds a target for the unit to attack. Target will usually be
 * the closest unit of different team. Passing in (-1, -1) will
 * call this method for all selected units.
 */

    void findTarget(int x, int y);

/**
 * Method: drawMap
 * Usage: this->gmap.drawMap(blitter);
 * -----------------------------------
 * Draws tiles and units to buffer.
 */

    void drawMap(Blitter *blitter);

/**
 * Method: updateMap
 * Usage in GameLoop: gmap->updateMap(blitter, timer);
 * ----------------------------------------------------------
 * Redraws the map to buffer with units at updated locations.
 */

    void updateMap(Blitter *blitter, Timer *timer);

/**
 * Destructor: ~GameMap
 */

    ~GameMap();

private:
    UnitManager units;
    TileManager tiles;
    ResourceManager resources;
    StructureManager structures;
    int width;
    int height;

/**
 * Private Method: inBounds
 * ------------------------
 * A helper function used in isPathable to check if the tile at
 * (x,y) is within the boundaries of the game map.
 */

    bool inBounds(int x, int y);

};

#endif //GAMEMAP_H
