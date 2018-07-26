#ifndef UNIT_H
#define UNIT_H

#include "MapObject.h"
#include "Timer.h"

class GameMap;

/**
 * Subclass: Unit
 * --------------
 * Stores information on a single unit in a game map.
 *
 * Methods: setTeam, getTeam, flagSelected, checkSelected, checkDead,
 *          setTarget, getTarget, clearTarget, chaseTarget, giveAttack,
 *          moveNext, findPath, blit, drawStraightPath, drawFullPath
 */

class Unit : public MapObject
{
    public:
        Unit();
        Unit(int x, int y, int team);

/**
 * Method: setTeam
 * Usage: unit.setTeam(0);
 * -------------------------
 * Sets the team of the unit.
 */

        void setTeam(int team);

/**
 * Method: getTeam
 * Usage in UnitManager: if (newUnit->getTeam() != 0)...
 * -----------------------------------------------------
 * Returns the team of the unit.
 */

        int getTeam();

/**
 * Method: flagSelected
 * Usage in UnitManager: selectedUnit->flagSelected(true);
 * ----------------------------------------------------------
 * Flags the unit as selected (or not), according to selected.
 */

        void flagSelected(bool selected);

/**
 * Method: checkSelected
 * Usage in UnitManager: if (this->all[pos]->checkSelected())...
 * -------------------------------------------------------------
 * Returns true if the unit is flagged as selected, false if not.
 */

        bool checkSelected();

/**
 * Method: checkDead
 * Usage in UnitManager: if (target->checkDead())...
 * -------------------------------------------------
 * Returns true if the unit is dead, false if alive.
 */

        bool checkDead();

/**
 * Method: setTarget
 * Usage in UnitManager: attacker->setTarget(best);
 * ------------------------------------------------
 * Sets the unit's target to provided pointer.
 */

        void setTarget(std::shared_ptr<Unit> target);

/**
 * Method: getTarget
 * Usage in UnitManager: target = current->getTarget();
 * -----------------------------------------------------------
 * Returns a pointer to the unit's target. Returns NULL if the
 * unit has no target.
 */

        std::shared_ptr<Unit> getTarget();

/**
 * Method: clearTarget
 * Usage in UnitManager: attacker->clearTarget();
 * ----------------------------------------------
 * Clears the units target by calling shared pointers reset method.
 */

        void clearTarget();

/**
 * Method: chaseTarget
 * Usage in UnitManager: current->chaseTarget(gmap, timer);
 * --------------------------------------------------------
 * Orders the unit to continue trying to reach its target,
 * usually resulting in moving one step closer to it. Returns
 * true if moving closer to unit, false if stuck.
 */

        bool chaseTarget(GameMap *gmap, Timer *timer);

/**
 * Method: giveAttack
 * Usage in UnitManager: current->giveAttack(timer->getTime());
 * ------------------------------------------------------------
 * Orders unit to attempt to attack its target. Will only
 * succeed if target is in range and unit is ready to attack
 * (based on attack speed). A successful attack will decrease
 * target's health.
 */

        void giveAttack(int currentTime);

/**
 * Method: moveNext
 * Usage in UnitManager: current->moveNext(gmap, timer);
 * -----------------------------------------------------
 * Updates unit to next position in current path. Will do
 * nothing if the the game is paused, the unit hasn't
 * recovered from its last move (based on movement speed),
 * or the unit has no current path. If the next tile is
 * occupied, the unit will first wait, then if that doesn't
 * work it will try to find a new path.
 */

        void moveNext(GameMap *gmap, Timer *timer);

/**
 * Method: findPath
 * Usage in UnitManager: unit->findPath(&dest, gmap, false);
 * ---------------------------------------------------------
 * Uses the A* pathfinding algorithm to find the shortest
 * path from the unit's location to the provided destination.
 * If checkUnits is true, then will try to find a path without
 * units in the way. Returns true if path found, false if not.
 */

        bool findPath(node *destinationNode, GameMap *gmap, bool checkUnits);

/**
 * Method: blit
 * Usage in UnitManager: current->blit(blitter);
 * ---------------------------------------------
 * Blits the unit to buffer. Also draws a selection circle if
 * selected and a health bar representing remaining health.
 */

        void blit(Blitter *blitter);

/**
 * Method: drawStraightPath
 * Usage in UnitManager: this->all[i]->drawStraightPath(blitter);
 * --------------------------------------------------------------
 * Draws a line to the unit's destination using Brenham's line
 * algorithm as implemented in Blitter.
 */

        void drawStraightPath(Blitter *blitter);

/**
 * Method: drawFullPath
 * Usage in UnitManager: this->all[i]->drawFullPath(blitter);
 * ----------------------------------------------------------
 * Draws out the unit's path tile by tile to its destination.
 */

        void drawFullPath(Blitter *blitter);

        virtual ~Unit();

    protected:
        std::string name;
        int attackSpeed;
        int moveSpeed;
        int health;
        int maxHealth;
        int attackDmg;

    private:
        int team;

        bool bloodied;
        bool dead;
        bool selected;

        int lastAttackTime;
        int lastMoveTime;
        int pathAttempts;
        int repathAttempts;

        int destX;
        int destY;
        std::shared_ptr<Unit> target;
        std::stack<node> currentPath;

/**
 * Private Method: makeNewNode
 * ---------------------------
 * A helper function for findPath. This function creates a new
 * node by copying a provided node and changing it's x and y
 * values by the provided amount. Thus one can make new nodes for
 * several different directions as seen in findPath.
 */

        void makeNewNode(int changeX, int changeY, const node *parent,
                         GameMap *gmap, std::vector<node> *nodes,
                         bool checkUnits);

/**
 * Private Method: setHeuristicValues
 * ----------------------------------
 * A helper function for findPath. Returns a node with values set
 * for distTravld, distToDest, and moveCost.
 */

        node setHeuristicValues(node curr, const node *parent, node *dest, GameMap *gmap);

/**
 * Private Method: findLowestCost
 * ------------------------------
 * A helper function for findPath. This function uses linear
 * search to find the node with the lowest (best) moveCost value,
 * which is used to determine the next best node to move to in
 * our implementation of the A* pathfinding algorithm.
 */

        node findLowestCost(std::set<node, nodeCmpFn> *s);

/**
 * Private Method: constructPath
 * -----------------------------
 * A helper function for findPath. This function produces a path
 * by tracing back through the parents of the goal node found by
 * findPath.
 */

        std::stack<node> constructPath(const node *root);

/**
 * Private Method: updatePath
 * --------------------------
 * Attempts to find an alternate path to the targets location. It
 * switches between allowing and disallowing units to be in the way.
 */

        void updatePath(GameMap *gmap);

/**
 * Private Method: receiveAttack
 * -----------------------------
 * Decreases unit's health by the amount of damage received.
 */

        void receiveAttack(int damage);

};

#endif // UNIT_H
