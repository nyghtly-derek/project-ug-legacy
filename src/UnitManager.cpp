#include "UnitManager.h"
#include "GameMap.h"
//#define PRINT_INFO

UnitManager::UnitManager()
{
}

void UnitManager::add(int x, int y, int team, int spriteID)
{
    if (team < 0)
    {
        if (spriteID == KNIGHT)
        {
            team = 1;
        }
        else
        {
            team = 0;
        }
    }

    #ifdef PRINT_INFO
    std::cout << "UnitManager::add -> creating unit2 at: (" << x << "," << y << ") with team: " << team << std::endl;
    #endif // PRINT_INFO

    std::shared_ptr<Unit> newUnit = NULL;
    if (spriteID == WORKER)
    {
        newUnit = std::make_shared<Goblin>(x, y, team);
    }
    else if (spriteID == KNIGHT)
    {
        newUnit = std::make_shared<Human>(x, y, team);
    }
    else
    {
        newUnit = std::make_shared<Unit>(x, y, team);
    }

    this->all.push_back(newUnit);
    if (newUnit->getTeam() != 0)
    {
        this->enemy.push_back(newUnit);

        #ifdef PRINT_INFO
        std::cout << "UnitManager::add -> enemy list size: " << this->enemy.size() << std::endl;
        #endif // PRINT_INFO
    }

    #ifdef PRINT_INFO
    std::cout << "UnitManager::add -> unit list size: " << this->all.size() << std::endl;
    #endif // PRINT_INFO
}

void UnitManager::destroy(int x, int y)
{
    bool attacking = false;
    bool enemy = false;
    bool selected = false;
    int pos = this->searchVectorOfUnits(x, y, this->all);
    if (pos >= 0)
    {
        #ifdef PRINT_INFO
        std::cout << "UnitManager::destroy -> destroying unit2 at: (" << x << "," << y << ")" << std::endl;
        #endif // PRINT_INFO

        // If unit is in the all list then we can check to see what
        // other lists it is in based on the unit's flags. This way
        // we can avoid redundant searches through other lists.
        if (this->all[pos]->getTarget() != NULL)
        {
            attacking = true;
        }
        if (this->all[pos]->getTeam() != 0)
        {
            enemy = true;
        }
        if (this->all[pos]->checkSelected())
        {
            selected = true;
        }
        this->all.erase(this->all.begin() + pos);

        #ifdef PRINT_INFO
        std::cout << "UnitManager::destroy -> unit all list size: " << this->all.size() << std::endl;
        #endif // PRINT_INFO
    }
    if (enemy == true)
    {
        this->removeFromVectorOfUnits(x, y, this->enemy);
    }
    if (attacking == true)
    {
        this->removeFromVectorOfUnits(x, y, this->attacking);
    }
    if (selected == true)
    {
        this->removeFromVectorOfUnits(x, y, this->selected);
    }
}

void UnitManager::select(int x, int y)
{
    int pos = this->searchVectorOfUnits(x, y, this->all);
    if (pos >= 0 && this->all[pos]->getTeam() == 0) // don't want to select units on other team
    {
        #ifdef PRINT_INFO
        std::cout << "UnitManager::select -> selecting unit2 at: (" << x << "," << y << ")" << std::endl;
        #endif // PRINT_INFO

        std::shared_ptr<Unit> selectedUnit = this->all[pos];
        this->selected.push_back(selectedUnit);
        selectedUnit->flagSelected(true);

        #ifdef PRINT_INFO
        std::cout << "UnitManager::select -> selected list size: " << this->selected.size() << std::endl;
        #endif // PRINT_INFO
    }
}

void UnitManager::deselect(int x, int y)
{
    if (x == -1 || y == -1)
    {
        #ifdef PRINT_INFO
        std::cout << "UnitManager::deselect -> deselecting all units" << std::endl;
        #endif // PRINT_INFO

        for (int i = 0; i < this->selected.size(); i++)
        {
            this->selected[i]->flagSelected(false);
        }
        this->selected.clear();

        #ifdef PRINT_INFO
        std::cout << "UnitManager::deselect -> selected list size: " << this->selected.size() << std::endl;
        #endif // PRINT_INFO
    }
    else
    {
        int pos = this->searchVectorOfUnits(x, y, this->selected);
        if (pos >= 0)
        {
            #ifdef PRINT_INFO
            std::cout << "UnitManager::deselect -> deselecting unit2 at: (" << x << "," << y << ")" << std::endl;
            #endif // PRINT_INFO

            this->selected[pos]->flagSelected(false);
            this->selected.erase(this->selected.begin() + pos);

            #ifdef PRINT_INFO
            std::cout << "UnitManager::deselect -> selected list size: " << this->selected.size() << std::endl;
            #endif // PRINT_INFO
        }
    }
}

void UnitManager::updateUnits(Blitter *blitter, GameMap *gmap, Timer *timer)
{
    resolveAttacks(gmap, timer);

    for (int i = 0; i < this->all.size(); i++)
    {
        std::shared_ptr<Unit> current = this->all[i];
        gmap->flagUnit(current->getX(), current->getY(), false);

        if (current->checkDead()) // in case a unit died, but not from a direct attack
        {
            this->destroy(current->getX(), current->getY());
            i--; // because the unit we were just on is now gone and the loop we don't want to double iterate the loop
        }
        else
        {
            if (current->getTeam() != 0)
            {
                if (current->getTarget() != NULL)
                {
                    if (!current->chaseTarget(gmap, timer)) // can't reach target
                    {
                        xypair target = this->chooseTarget(current);
                        if (target.x < 0 && target.y < 0) // no target available
                        {
                            this->removeAttacker(current);
                        }
                    }
                }
                else
                {
                    xypair target = this->chooseTarget(current);
                    if (target.x >= 0 && target.y >= 0) // target found
                    {
                        this->attacking.push_back(current);
                    }
                }
            }
            else if(current->getTarget() != NULL)
            {
                current->chaseTarget(gmap, timer);
            }
            else
            {
                current->moveNext(gmap, timer);
            }

            gmap->flagUnit(current->getX(), current->getY(), true);
            current->blit(blitter);
        }
    }

    #ifdef DRAW_INFO
    this->drawDestinations(blitter, true);
    #else
    //this->drawDestinations(blitter, false); TODO: doesn't work properly
    #endif
}

void UnitManager::findPath(GameMap *gmap, int x, int y, int destX, int destY)
{
    #ifdef PRINT_INFO
    std::cout << "UnitManager::findPath -> unit manager findPath running..." << std::endl;
    #endif // PRINT_INFO

    if ( x < 0 && y < 0 )
    {
        for (int i = 0; i < this->selected.size(); i++)
        {
            #ifdef PRINT_INFO
            std::cout << "UnitManager::findPath -> pathfinding for selected unit(s) " << i << std::endl;
            #endif // PRINT_INFO

            this->findPath(gmap, this->selected[i]->getX(), this->selected[i]->getY(), destX, destY);
        }
        return;
    }


    if (gmap->checkUnit(x,y))
    {
        std::shared_ptr<Unit> unit = this->getUnit(x, y);
        int pos = this->searchVectorOfUnits(unit->getX(), unit->getY(), this->attacking);

        if (unit->getTeam() == 0)
        {
            if (gmap->checkUnit(destX, destY))
            {
                if (pos >= 0)
                {
                    bool targetReset = this->resetAttacker(unit, destX, destY);
                    if (targetReset) { return; }
                }
                else
                {
                    bool targetFound = this->setAttacker(unit, destX, destY); // this is where player's input results in a target
                    if (targetFound) { return; }
                }
            }
            if (pos >= 0)
            {
                this->removeAttacker(unit);
            }
        }

        node dest = {};
        dest.x = destX;
        dest.y = destY;

        #ifdef PRINT_INFO
        std::cout << "UnitManager::pathfinder -> pathfinding unit at: (" << x << "," << y << ")" << std::endl;
        #endif // PRINT_INFO

        //if (unit->findPath(&dest, this->mdat, true) == false) // units considered unpathable
            unit->findPath(&dest, gmap, false); // walls considered unpathable
    }
}

void UnitManager::findTarget(GameMap *gmap, int x, int y)
{
    if ( x < 0 && y < 0 )
    {
        for (int i = 0; i < this->selected.size(); i++)
        {
            #ifdef PRINT_INFO
            std::cout << "UnitManager::findTarget -> finding targets for selected unit(s) " << i << std::endl;
            #endif // PRINT_INFO

            this->findTarget(gmap, this->selected[i]->getX(), this->selected[i]->getY());
        }
        return;
    }

    if (gmap->checkUnit(x,y))
    {
        std::shared_ptr<Unit> unit = this->getUnit(x, y);
        int pos = this->searchVectorOfUnits(unit->getX(), unit->getY(), this->attacking);

        xypair dest = this->chooseTarget(unit);
        if (dest.x >= 0 && dest.y >= 0)
        {
            if (gmap->checkUnit(dest.x, dest.y))
            {
                if (pos >= 0)
                {
                    bool targetReset = this->resetAttacker(unit, dest.x, dest.y);
                    if (targetReset) { return; }
                }
                else
                {
                    bool targetFound = this->setAttacker(unit, dest.x, dest.y);
                    if (targetFound) { return; }
                }
            }
        }
        else
        {
            if (pos >= 0)
            {
                this->removeAttacker(unit);
            }
        }
    }
}


void UnitManager::drawDestinations(Blitter *blitter, bool fullPath)
{
    if (fullPath)
    {
        for (int i = 0; i < this->all.size(); i++)
        {
            this->all[i]->drawFullPath(blitter);
        }
        return;
    }
    for (int i = 0; i < this->all.size(); i++)
    {
        this->all[i]->drawStraightPath(blitter);
    }
}

UnitManager::~UnitManager()
{}

/*
 * Private Methods
 */

std::shared_ptr<Unit> UnitManager::getUnit(int x, int y)
{
    int pos = this->searchVectorOfUnits(x, y, this->all);
    return this->all[pos];
}

int UnitManager::searchVectorOfUnits(int x, int y, std::vector<std::shared_ptr<Unit>> units)
{
    for (int i = 0; i < units.size(); i++)
    {
        if (units[i]->getX() == x && units[i]->getY() == y)
        {
            return i;
        }
    }
    return -1;
}

bool UnitManager::removeFromVectorOfUnits(int x, int y, std::vector<std::shared_ptr<Unit>> units)
{
    int pos = this->searchVectorOfUnits(x, y, units);
    if (pos >= 0)
    {
        units.erase(units.begin() + pos);
        return true;
    }
    return false;
}

bool UnitManager::setAttacker(std::shared_ptr<Unit> attacker, int targetX, int targetY)
{
    if (targetX < 0 || targetY < 0)
    {
        xypair target = this->chooseTarget(attacker);
        if (target.x >= 0 && target.y >= 0)
        {
            this->attacking.push_back(attacker);
            return true;
        }
    }
    else
    {
        int pos = this->searchVectorOfUnits(targetX, targetY, this->all);
        if (pos >= 0)
        {
            if (this->all[pos]->getTeam() != attacker->getTeam())
            {
                attacker->setTarget(this->all[pos]);
                this->attacking.push_back(attacker);
                return true;
            }
        }
    }
    return false;
}

bool UnitManager::removeAttacker(std::shared_ptr<Unit> attacker)
{
    for (int i = 0; i < this->attacking.size(); i++)
    {
        if (this->attacking[i] == attacker)
        {
            attacker->clearTarget();
            this->attacking.erase(this->attacking.begin() + i);
            return true;
        }
    }
    return false;
}

bool UnitManager::resetAttacker(std::shared_ptr<Unit> attacker, int targetX, int targetY)
{
    if (this->removeAttacker(attacker) == true)
    {
        return this->setAttacker(attacker, targetX, targetY);
    }
    return false;
}

xypair UnitManager::chooseTarget(std::shared_ptr<Unit> attacker)
{
    #ifdef PRINT_INFO
    std::cout << "UnitManager::chooseTarget -> choosing target for unit at: (" << attacker->getX() << "," << attacker->getY() << ")" << std::endl;
    #endif // PRINT_INFO

    if (attacker == NULL)
    {
        return {-1, -1};
    }

    std::shared_ptr<Unit> best = NULL;
    float bestDist = -1;
    for (int i = 0; i < this->all.size(); i++)
    {
        std::shared_ptr<Unit> defender = this->all[i];
        #ifdef PRINT_INFO
        //std::cout << "UnitManager::chooseTarget -> possible target at: (" << defender->getX() << "," << defender->getY() << ")" << std::endl;
        #endif // PRINT_INFO

        if (attacker != defender)
        {
            #ifdef PRINT_INFO
            //std::cout << "UnitManager::chooseTarget -> attacker not same as defender..." << std::endl;
            #endif // PRINT_INFO

            if (defender->getTeam() != attacker->getTeam()) // IF teams are not the same
            {
                #ifdef PRINT_INFO
                //std::cout << "UnitManager::chooseTarget -> attacker and defender different teams..." << std::endl;
                #endif // PRINT_INFO

                xypair attackerPos = {attacker->getX(), attacker->getY()};
                xypair defenderPos = {defender->getX(), defender->getY()};
                float thisDist = distBtwnPnts(attackerPos, defenderPos);
                if ( (thisDist < bestDist) // IF new distance is smaller than previous best
                    && (bestDist >= 0) ) // AND previous best is not initial placeholder
                {
                    best = defender;
                    bestDist = thisDist;
                }
                else if (bestDist == -1) // IF first unit to be found, set first value
                {
                    best = defender;
                    bestDist = thisDist;
                }
            }
        }
    }
    if (best != NULL)
    {
        #ifdef PRINT_INFO
        //std::cout << "UnitManager::chooseTarget -> target found!" << std::endl;
        #endif // PRINT_INFO

        attacker->setTarget(best);
        return {best->getX(), best->getY()};
    }
    return {-1, -1};
}

void UnitManager::resolveAttacks(GameMap *gmap, Timer *timer)
{
    std::set<xypair, pntCmpFn> dying = {}; // set to prevent destroying units more than once and crashing program
    std::shared_ptr<Unit> current = NULL;
    std::shared_ptr<Unit> target = NULL;
    for (int i = 0; i < this->attacking.size(); i++)
    {
        current = this->attacking[i];
        target = current->getTarget();
        if (target->checkDead())
        {
            xypair dyingUnit = {target->getX(), target->getY()};
            dying.insert(dyingUnit);
            this->removeAttacker(current);
        }
        else
        {
            current->giveAttack(timer->getTime());
            if (target->checkDead())
            {
                // seems redundant, but prevents units from attacking dead things,
                // and from persisting as an attacker even if their target is dead.
                xypair dyingUnit = {target->getX(), target->getY()};
                dying.insert(dyingUnit);
                this->removeAttacker(current);
            }
        }


    }
    for(std::set<xypair>::iterator it = dying.begin(); it != dying.end(); it++)
    {
        #ifdef PRINT_INFO
        std::cout << "UnitManager::resolveAttacks -> calling destroy for unit at: (" << (*it).x << "," << (*it).y << ")..." << std::endl;
        #endif // PRINT_INFO

        // dying units aren't destroyed until after all attacks are
        // resolved to avoid issues with attacking non-existent units
        gmap->removeUnit((*it).x, (*it).y);
    }
}
