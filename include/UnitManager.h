
#ifndef UNITMANAGER_H
#define UNITMANAGER_H

#include "Goblin.h"
#include "Human.h"

class GameMap;

/**
 * Class: UnitManager
 * ------------------
 * This class stores information for units. A client can add and
 * destroy units, pathfind for units, draw units, manage attacks
 * between units, and update unit locations, as well as some other
 * functions.
 *
 * Methods: add, destroy, select, deselect, updateUnits, findPath,
 *          drawDestinations
 */

class UnitManager
{
    public:

/**
 * Constructor: UnitManager
 * Usage in :
 * -------------------
 */

        UnitManager();

/**
 * Method: add
 * Usage in :
 * ----------------
 */

        void add(int x, int y, int team, int spriteID);

/**
 * Method: destroy
 * Usage in :
 * -------------------
 */

        void destroy(int x, int y);

/**
 * Method: select
 * Usage in :
 * -------------------
 */

        void select(int x, int y);

/**
 * Method: deselect
 * Usage in :
 * ------------------
 */

        void deselect(int x, int y);

/**
 * Method: updateUnits
 * Usage in :
 * -------------------
 */

        void updateUnits(Blitter *blitter, GameMap *gmap, Timer *timer);

/**
 * Method: findPath
 * Usage in :
 * -------------------
 */

        void findPath(GameMap *gmap, int x, int y, int destX, int destY);

        void findTarget(GameMap *gmap, int x, int y);

/**
 * Method: drawDestinations
 * Usage:
 * ------------------------------------
 * Draws a line from all units to their respective destinations. If
 * client passes in true for fullPath, then the lines will outline
 * each tile in tile in their paths rather than be straight.
 */

        void drawDestinations(Blitter *blitter, bool fullPath);

/**
 * Destructor: ~UnitManager
 */

        ~UnitManager();

    private:
        std::vector<std::shared_ptr<Unit>> all;
        std::vector<std::shared_ptr<Unit>> enemy;
        std::vector<std::shared_ptr<Unit>> selected;
        std::vector<std::shared_ptr<Unit>> attacking;

/**
 * Private Method: getUnit
 * -----------------------
 * Returns a pointer to the unit at position (x, y).
 */

        std::shared_ptr<Unit> getUnit(int x, int y);

/**
 * Private Method: searchVectorOfUnits
 * -----------------------------------
 * Uses linear search to find and return the position of a given
 * unit. If the unit is not in the list, then returns -1.
 */

        int searchVectorOfUnits(int x, int y, std::vector<std::shared_ptr<Unit>> units);

/**
 * Private Method: removeFromVectorOfUnits
 * ---------------------------------------
 * Removes unit at position (x,y) from provided list. If unit is not
 * in list, returns false.
 */

        bool removeFromVectorOfUnits(int x, int y, std::vector<std::shared_ptr<Unit>> units);

/**
 * Private Method: setAttacker
 * ---------------------------
 * Sets the target for attacker unit to be the unit at location
 * (targetX, targetY). If the unit there is on the same team as
 * attacker then the target will not be set. If values less than
 * zero are passed in for targetX and targetY than chooseTarget will
 * decide the target. Attacker is added to the attacking list.
 */

        bool setAttacker(std::shared_ptr<Unit> attacker, int targetX, int targetY);

/**
 * Private Method: removeAttacker
 * ------------------------------
 * Removes attacker from the attacking list and clears its target.
 */
        bool removeAttacker(std::shared_ptr<Unit> attacker);

/**
 * Private Method: resetAttacker
 * ------------------------------
 * If attacker is already in the attacking list, calls
 * removeAttacker and setAttacker with the provided variables.
 */

        bool resetAttacker(std::shared_ptr<Unit> attacker, int targetX, int targetY);

/**
 * Private Method: chooseTarget
 * ------------------------------
 * Sets the attacker's target to the closest unit on a different
 * team.
 */

        xypair chooseTarget(std::shared_ptr<Unit> attacker);

/**
 * Private Method: resolveAttacks
 * ------------------------------
 * Cycles through attacking units and resolves their attacks. If
 * target dies then the attacking unit is removed from the attacking
 * list. At the end of the cycle all dead units are destroyed using
 * MatchData's removeUnit function.
 */

        void resolveAttacks(GameMap *gmap, Timer *timer);
};

#endif // UNITMANAGER_H
