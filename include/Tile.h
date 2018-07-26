#ifndef TILE_H
#define TILE_H

#include "MapObject.h"

/**
 * Class: Tile
 * -----------
 * This class stores information on a single tile in a game map.
 *
 * Methods: flagWall, checkWall, flagUnit, checkUnit, setID
 */

class Tile : public MapObject
{
    public:
        Tile();
        Tile(int x, int y, int tileID);

/**
 * Method: flagWall
 * Usage in TileManager: this->tiles[{x,y}].flagUnit(hasUnit);
 * -----------------------------------------------------------
 * Flags the tile according to isWall. If true, then units
 * may not step on this tile. If false, then they may.
 */

        void flagWall(bool isWall);

/**
 * Method: checkWall
 * Usage in TileManager: if (!this->tiles[{x,y}].checkWall())...
 * -------------------------------------------------------------
 * Returns true if wall, false if not. Tiles that are walls
 * cannot have units on them.
 */

        bool checkWall();

/**
 * Method: flagUnit
 * Usage in TileManager: this->tiles.flagUnit(x, y, hasUnit);
 * ----------------------------------------------------------
 * Flags the tile as having a unit, according to hasUnit. Only
 * one unit can be on a tile at one time.
 */

        void flagUnit(bool hasUnit);

/**
 * Method: checkUnit
 * Usage in TileManager: return this->tiles[{x,y}].checkUnit();
 * ------------------------------------------------------------
 * Returns true if unit on tile, false if not.
 */

        bool checkUnit();

/**
 * Method: setID
 * Usage: this->setID(STONE);
 * ---------------------------------------
 * Sets the tile's ID according to tileID.
 */

        void setID(int tileID);

        virtual ~Tile();

    protected:

    private:
        bool isWall;
        bool hasUnit;
};

#endif // TILE_H
