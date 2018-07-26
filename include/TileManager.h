#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include "Tile.h"

/**
 * Class: TileManager
 * ------------------
 * This class stores information on a collection of tiles for
 * a game map.
 *
 * Methods: add, flagUnit, checkUnit, flagWall, checkWall,
 *          checkPathable,blitAll
 */

class TileManager
{
    public:
        TileManager();

/**
 * Method: add
 * Usage in GameMap: this->tiles.add(x, y, ID);
 * --------------------------------------------
 * Adds a new tile to the collection.
 */

        void add(int x, int y, int tileID);

/**
 * Method: flagUnit
 * Usage in GameMap: this->tiles.flagUnit(x, y, hasUnit);
 * -----------------------------------------------------------
 * Flags the tile at (x,y) as having a unit, according to hasUnit.
 * Only one unit can be on a tile at one time.
 */

        void flagUnit(int x, int y, bool hasUnit);

/**
 * Method: checkUnit
 * Usage in GameMap: return this->tiles.checkUnit(x, y);
 * -----------------------------------------------------
 * Returns true if unit on tile at (x,y), false if not.
 */

        bool checkUnit(int x, int y);

/**
 * Method: flagWall
 * Usage: this->flagWall(10, 7, true);
 * ----------------------------------------------------------
 * Flags the tile at (x,y) according to isWall. If true, then
 * units may not step on this tile. If false, then they may.
 */

        void flagWall(int x, int y, bool isWall);

/**
 * Method: checkWall
 * Usage: this->checkWall(10, 8);
 * ----------------------------------------------------
 * Returns true if tile at (x,y) is wall, false if not.
 * Tiles that are walls cannot have units on them.
 */

        bool checkWall(int x, int y);

/**
 * Method: checkPathable
 * Usage in GameMap:
 *  return this->tiles.checkPathable(x, y, checkUnits);
 * ----------------------------------------------------
 * Returns true if tile at (x,y) can be occupied by a unit. If
 * checkUnits is true, then the presence of a unit is taken
 * into account (only one unit can be on a tile at once).
 */

        bool checkPathable(int x, int y, bool checkUnits);

/**
 * Method: blitAll
 * Usage in GameMap: this->tiles.blitAll(blitter);
 * -----------------------------------------------
 * Blits all tiles in collection to buffer.
 */

        void blitAll(Blitter *blitter);

        virtual ~TileManager();

    private:
        std::map<xypair, Tile, pntCmpFn> tiles;

};

#endif // TILEMANAGER_H
