#include "GameMap.h"

GameMap::GameMap()
{
    UnitManager uman;
    this->units = uman;
    TileManager tiles;
    this->tiles = tiles;
    ResourceManager resources;
    this->resources = resources;
    StructureManager structures;
    this->structures = structures;
}

bool GameMap::loadMap(std::string mapFilename, Blitter *blitter)
{
    std::string line;
    int width;
    int height;
    std::ifstream mapFile;
    mapFile.open(mapFilename);
    if (mapFile.fail())
    {
        std::cout << "File at location/of name " << mapFilename << " failed to open." << std::endl;
        return false;
    }
    mapFile >> width >> height;
    this->width = width;
    this->height = height;
    blitter->createBuffer(0, width * 32, height * 32, 32, 0, 0, 0, 0xff000000);
    for (int y = 0; y < height; y++)
    {
        mapFile >> line;
        for (int x = 0; x < width; x++)
        {
            // tile info block
            int ID = atoi(line.substr(x, 1).c_str());
            this->tiles.add(x, y, ID);
        }
    }
    for (int y = 0; y < height; y++)
    {
        mapFile >> line;
        for (int x = 0; x < width; x++)
        {
            // unit info block
            int ID = atoi(line.substr(x, 1).c_str());
            if (ID != 0) // 0 in map file denotes no unit in that location
            {
                this->units.add(x, y, -1, ID);
            }
        }
    }
    for (int y = 0; y < height; y++)
    {
        mapFile >> line;
        for (int x = 0; x < width; x++)
        {
            // unit info block
            int ID = atoi(line.substr(x, 1).c_str());
            if (ID != 0) // 0 in map file denotes no resource in that location
            {
                this->resources.add(x, y, ID);
            }
        }
    }
    for (int y = 0; y < height; y++)
    {
        mapFile >> line;
        for (int x = 0; x < width; x++)
        {
            // unit info block
            int ID = atoi(line.substr(x, 1).c_str());
            if (ID != 0) // 0 in map file denotes no structure in that location
            {
                this->structures.add(x, y, 0, ID);
                #ifdef PRINT_INFO
                std::cout << "GameMap::LoadMap -> Structure added." << std::endl;
                #endif // PRINT_INFO
            }
        }
    }
    mapFile.close();
    return true;
}

int GameMap::getWidth()
{
    return this->width;
}

int GameMap::getHeight()
{
    return this->height;
}

void GameMap::flagUnit(int x, int y, bool hasUnit)
{
    this->tiles.flagUnit(x, y, hasUnit);
}

bool GameMap::checkUnit(int x, int y)
{
    return this->tiles.checkUnit(x, y);
}

bool GameMap::checkPathable(int x, int y, bool checkUnits)
{
    if (this->inBounds(x, y) == false)
    {
        #ifdef PRINT_INFO
        std::cout << "GameMap::checkPathable -> Tile not in bounds." << std::endl;
        #endif // PRINT_INFO
        return false;
    }
    if (this->resources.checkResource(x, y))
    {
        #ifdef PRINT_INFO
        std::cout << "GameMap::checkPathable -> Resource on tile." << std::endl;
        #endif // PRINT_INFO
        return false;
    }
    if (!this->structures.checkPathable(x, y))
    {
        #ifdef PRINT_INFO
        std::cout << "GameMap::checkPathable -> Structure on tile not pathable." << std::endl;
        #endif // PRINT_INFO
        return false;
    }
    return this->tiles.checkPathable(x, y, checkUnits);
}

void GameMap::removeUnit(int x, int y)
{
    if (!this->checkUnit(x, y))
    {
        this->units.destroy(x, y);
        this->flagUnit(x, y, false);
    }
}

void GameMap::select(int x, int y)
{
    this->units.select(x, y);
}

void GameMap::deselect(int x, int y)
{
    this->units.deselect(x, y);
}

void GameMap::findPath(int x, int y, int destX, int destY)
{
    this->units.findPath(this, x, y, destX, destY);
}

void GameMap::findTarget(int x, int y)
{
    this->units.findTarget(this, x, y);
}

void GameMap::drawMap(Blitter *blitter)
{
    this->tiles.blitAll(blitter);
    this->resources.blitAll(blitter);
    this->structures.blitAll(blitter);
}

void GameMap::updateMap(Blitter *blitter, Timer *timer)
{
    blitter->clearScreen();
    this->drawMap(blitter);
    this->units.updateUnits(blitter, this, timer);
}

GameMap::~GameMap()
{}

/*
 * Private Methods
 * ---------------
 */

bool GameMap::inBounds(int x, int y)
{
    if (x < this->width && x >= 0 && y < this->height && y >= 0)
        return true;
    return false;
}


