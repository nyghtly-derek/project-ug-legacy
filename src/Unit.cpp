#include "Unit.h"
#include "GameMap.h"

Unit::Unit() : MapObject()
{
    this->name = "Unit";
    this->setTeam(0);
    this->setID(WORKER);
    this->attackSpeed = 250;
    this->moveSpeed = 250;

    this->bloodied = false;
    this->dead = false;
    this->selected = false;

    this->lastAttackTime = 250;
    this->lastMoveTime = 250;
    this->pathAttempts = 0;
    this->repathAttempts = 0;

    this->destX = -1;
    this->destY = -1;
    this->target = NULL;
    this->currentPath = {};
}

Unit::Unit(int x, int y, int team)
    : Unit()
{
    this->setX(x);
    this->setY(y);
    this->setTeam(team);
}

void Unit::setTeam(int team)
{
    this->team = team;
}

int Unit::getTeam()
{
    return this->team;
}

void Unit::flagSelected(bool selected)
{
    this->selected = selected;
}

bool Unit::checkSelected()
{
    return this->selected;
}

bool Unit::checkDead()
{
    return this->dead;
}

void Unit::setTarget(std::shared_ptr<Unit> target)
{
    this->target = target;
}

std::shared_ptr<Unit> Unit::getTarget()
{
    return this->target;
}

void Unit::clearTarget()
{
    this->target.reset();
}

bool Unit::chaseTarget(GameMap *gmap, Timer *timer)
{
    if (this->target != NULL)
    {
        node dest = {};
        dest.x = this->target->getX();
        dest.y = this->target->getY();

        if (this->getTeam() != 0)
        {
            if (!this->findPath(&dest, gmap, true))
            {
                return false;
            }
            else
            {
                this->moveNext(gmap, timer);
            }
        }
        else if (this->target->getX() == this->destX && this->target->getY() == this->destY) // i.e. target hasn't moved
        {
            this->moveNext(gmap, timer);
        }
        else
        {
            this->findPath(&dest, gmap, false);
        }
        return true;
    }
    return false;
}

void Unit::giveAttack(int currentTime)
{
    xypair targetPair = {this->target->getX(), this->target->getY()};
    xypair thisPair = {this->getX(), this->getY()};
    if ( testAdjacency(targetPair, thisPair) && (!this->dead) )
    {
        if ((currentTime - this->attackSpeed) <= this->lastAttackTime)
        {
            return;
        }
        #ifdef PRINT_INFO
        std::cout << "Unit::giveAttack -> "<< this->name << " at position (" << this->getX() << ", " << this->getY() << ") attacked something!" << std::endl;
        #endif // PRINT_INFO

        this->target->receiveAttack(this->attackDmg);
        this->lastAttackTime = currentTime;
    }
}

void Unit::moveNext(GameMap *gmap, Timer *timer)
{
    if (timer->checkPause())
    {
        return;
    }
    int currentTime = timer->getTime();
    if ((currentTime - this->moveSpeed) <= this->lastMoveTime)
    {
        return;
    }
    if (this->currentPath.empty())
    {
        return;
    }
    if (this->pathAttempts < 3)
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::moveNext -> # of path attempts: " << this->pathAttempts << std::endl;
        #endif // PRINT_INFO

        node next = this->currentPath.top();

        if (gmap->checkPathable(next.x, next.y, true))
        {
            this->setX(next.x);
            this->setY(next.y);
            this->currentPath.pop();
        }
        else
        {
            // path failed, so increment attempt
            this->pathAttempts++;
        }
        this->lastMoveTime = currentTime;
        return;
    }
    else
    {
        // move attempts failed too many times, so attempt to repath
        updatePath(gmap);
    }
}

bool Unit::findPath(node *destinationNode, GameMap *gmap, bool checkUnits)
{
    // using A* algorithm with small modifications

    node origin = {NULL, this->getX(), this->getY(), 0, 0, 0};

    this->destX = destinationNode->x;
    this->destY = destinationNode->y;

    bool destPossible = true;
    if (!gmap->checkPathable(destinationNode->x, destinationNode->y, false))
    {
        destPossible = false;
        if (this->target != NULL)
        {
            // see note at Unit::makeNewNode about why this exception is made
            destPossible = true;
        }
    }

    if (!destPossible)
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::findPath -> Destination not possible." << std::endl;
        #endif // PRINT_INFO
        return destPossible;
    }

    std::set<node, nodeCmpFn> open;
    std::set<node, nodeCmpFn> closed;
    open.insert(origin);

    while (!open.empty())
    {
        node parent0 = this->findLowestCost(&open);
        open.erase(open.find(parent0));
        closed.insert(parent0);
        const node *parent = &(*closed.find(parent0));

        std::vector<node> nodes;

        this->makeNewNode(1, 0, parent, gmap, &nodes, checkUnits); // E
        this->makeNewNode(-1, 0, parent, gmap, &nodes, checkUnits); // W

        this->makeNewNode(0, 1, parent, gmap, &nodes, checkUnits); // S
        this->makeNewNode(0, -1, parent, gmap, &nodes, checkUnits); // N

        //this->newNode(1, 1, gmap, &nodes, checkUnits);  // SE
        //this->newNode(-1, -1, gmap, &nodes, checkUnits); // NW

        //this->newNode(-1, 1, gmap, &nodes, checkUnits); // SW
        //this->newNode(1, -1, gmap, &nodes, checkUnits); // SE

        // use dummy because nodes may decrease in size
        int dummy = nodes.size();
        for (int i = 0; i < dummy; i++)
        {
            node curr = nodes.back();
            nodes.pop_back();

            if (curr.x == destinationNode->x && curr.y == destinationNode->y)
            {
                this->currentPath = this->constructPath(&curr); // path found!
                return true;
            }

            curr = this->setHeuristicValues(curr, parent, destinationNode, gmap);
//            curr.distTravld = parent->distTravld + distBtwnPnts(curr, *parent);
//            curr.distToDest = distBtwnPnts(curr, *destinationNode);
//            curr.moveCost = curr.distTravld + curr.distToDest;

            bool addToOpen = true;

            if (open.find(curr) != open.end())
            {
                node currInSet = *(open.find(curr));
                if (currInSet.moveCost < curr.moveCost)
                    addToOpen = false;
            }

            if (closed.find(curr) != closed.end())
            {
                node currInSet = *(closed.find(curr));
                if (currInSet.moveCost < curr.moveCost)
                    addToOpen = false;
            }

            if (addToOpen == true)
            {
                open.insert(curr);
            }
        }
    }
    return false;
}

void Unit::blit(Blitter *blitter)
{
    if (this->dead)
    {
        return; // TODO: can change later to draw death sprite instead
    }
    if (this->selected)
    {
        blitter->blitSprite(CIRCLE, this->getX() * 32, this->getY() * 32, true);
    }

    // TODO: put the bar drawing into a function

    blitter->blitSprite(this->objectID, this->getX() * 32, this->getY() * 32, true);

    xypair healthBarDestination = {(this->getX())*32 + 2, (this->getY())*32 - 4};
    xypair healthBarDimension = {28, 4};

    blitter->drawEmptyRect(healthBarDestination, healthBarDimension, 0x00FF00, true);

    float healthBarRemaining = healthBarDimension.x * ((float)this->health / this->maxHealth);
    healthBarDimension.x = healthBarRemaining;

    blitter->blitFilledRect(healthBarDestination, healthBarDimension, 0x00FF00, 0, true);

    #ifdef PRINT_INFO
    //std::cout << healthBarDimension.x << " * (" << this->health << " / " << this->maxHealth << ") = " << healthBarRemaining << std::endl;
    #endif // PRINT_INFO
}

void Unit::drawStraightPath(Blitter *blitter)
{
    if (!this->currentPath.empty())
    {
        // *32 scales the line to the size of the map (32x32 tiles)
        // +16 slides the line so that the beginning and end will be in the middle of their respective tiles
        blitter->drawLine(this->getX() * 32 + 16, this->getY() * 32 + 16, this->destX * 32 + 16,
                          this->destY * 32 + 16, 0x00FF00, true);
    }
}

void Unit::drawFullPath(Blitter *blitter)
{
    if (!this->currentPath.empty())
    {
        std::stack<node> path = this->currentPath;
        node previous = {NULL, this->getX(), this->getY(), 0, 0, 0};
        while (!path.empty())
        {
            // *32 scales the line to the size of the map (32x32 tiles)
            // +16 slides the line so that the beginning and end will be in the middle of their respective tiles
            blitter->drawLine(previous.x * 32 + 16, previous.y * 32 + 16,
                              path.top().x * 32 + 16, path.top().y * 32 + 16, 0x00FF00, true);
            previous = path.top();
            path.pop();
        }
    }
}

Unit::~Unit()
{}

/*
 * Private Methods
 * ---------------
 */

void Unit::makeNewNode(int changeX, int changeY, const node *parent, GameMap *gmap, std::vector<node> *nodes, bool checkUnits)
{
    node newNode = *(parent);
    newNode.parent = parent;
    newNode.x = newNode.x + changeX;
    newNode.y = newNode.y + changeY;
    if (newNode.x == this->destX
        && newNode.y == this->destY)
    {
        nodes->push_back(newNode);
        return;
    }
    if (gmap->checkPathable(newNode.x, newNode.y, checkUnits))
    {
        nodes->push_back(newNode);
    }
}

node Unit::setHeuristicValues(node curr, const node *parent, node *dest, GameMap *gmap)
{
    curr.distTravld = parent->distTravld + distBtwnPnts(curr, *parent);
    curr.distToDest = distBtwnPnts(curr, *dest);
    curr.moveCost = curr.distTravld + curr.distToDest;
    if (gmap->checkUnit(curr.x,curr.y) == true)
    {
        curr.moveCost *= 1.5;
    }
    return curr;
}


node Unit::findLowestCost(std::set<node, nodeCmpFn> *s)
{
    node best = *(s->begin());
    std::set<node, nodeCmpFn>::iterator i;
    for (i = s->begin(); i != s->end(); i++)
    {
        node curr = *i;
        if (curr.moveCost < best.moveCost)
            best = curr;
    }
    return best;
}

std::stack<node> Unit::constructPath(const node *root)
{
    std::stack<node> path;
    while (true)
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::constructPath -> Current: (" << root->x << "," << root->y << ")" << std::endl;
        #endif // PRINT_INFO

        path.push(*root);
        if (root->parent == NULL)
        {
            break;
        }

        #ifdef PRINT_INFO
        std::cout << "Unit::constructPath -> Parent: (" << root->parent->x << "," << root->parent->y << ")" << std::endl;
        #endif // PRINT_INFO

        root = root->parent;
    }
    // pop to remove start location, because unit is already there
    path.pop();
    return path;
}

void Unit::updatePath(GameMap *gmap)
{
    if (this->repathAttempts > 25)
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::updatePath -> all attempts failed " << this->repathAttempts << std::endl;
        #endif // PRINT_INFO
        clearNodeStack(&this->currentPath);
        this->repathAttempts = 0;
        this->pathAttempts = 0;
        return;
    }
    if (this->repathAttempts % 2 == 0)
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::updatePath -> attempting to find full clear path" << this->repathAttempts << std::endl;
        #endif // PRINT_INFO
        node destination = {NULL, this->destX, this->destY, 0, 0, 0};
        this->findPath(&destination, gmap, true); // find new path without units or structures in the way
        this->pathAttempts = 0;
        this->repathAttempts++;
    }
    else
    {
        #ifdef PRINT_INFO
        std::cout << "Unit::updatePath -> attempting to find path without structures in the way" << this->repathAttempts << std::endl;
        #endif // PRINT_INFO
        node destination = {NULL, this->destX, this->destY, 0, 0, 0};
        this->findPath(&destination, gmap, false); // find new path with units but without structures in the way
        this->pathAttempts = 0;
        this->repathAttempts++;
    }
}

void Unit::receiveAttack(int damage)
{

    if (!this->dead)
    {
        this->health -= damage;
        #ifdef PRINT_INFO
        std::cout << "Unit::receiveAttack -> "<< this->name << " at position (" << this->getX() << ", " << this->getY() << ") took " << damage << " damage!" << std::endl;
        #endif // PRINT_INFO
        if (this->health <= 0)
        {
            #ifdef PRINT_INFO
            std::cout << "Unit::receiveAttack -> "<< this->name << " at position (" << this->getX() << ", " << this->getY() << ") is dead!" << std::endl;
            #endif // PRINT_INFO
            this->dead = true;
            this->bloodied = false;
            return;
        }
        else if (this->health <= (this->maxHealth / 2))
        {
            #ifdef PRINT_INFO
            std::cout << "Unit::receiveAttack -> "<< this->name << " at position (" << this->getX() << ", " << this->getY() << ") is bloodied!" << std::endl;
            #endif // PRINT_INFO
            this->bloodied = true;
            return;
        }
    }
}
