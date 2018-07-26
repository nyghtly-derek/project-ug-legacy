/*
main.cpp

Loading a map and engaging in combat

By: Derek Blankenship

Used:
-SDL library for drawing
-Code Blocks for development
-TileStudio to make bitmap file
-GitHub for repository
*/

#include "Additional.h"
#include "Blitter.h"
#include "GameMap.h"
#include "Camera.h"
#include "Timer.h"
//#define PRINT_INFO

/**
 * Function: initSpriteLocs
 * Usage: initSpriteLocs(bitmapLocs);
 * ----------------------------------
 * Loads in the locations and dimensions of all sprites in the
 * sprite sheet for this project. The user may then access this
 * array for the drawing of specific sprites.
 */

void initSpriteLocs(spriteSrcLoc *locsArray, int numSprites);

void setSrcLoc(int spriteID, int x, int y, int h, int w, spriteSrcLoc *locsArray);

int mainMenuLoop(Blitter *blitter, SDL_Window *window);

int loadMatch(Blitter *blitter, GameMap *gmap, SDL_Window *window);

int matchLoop(GameMap *gmap, Camera *camera, Blitter *blitter, Timer *timer, SDL_Window *window);

/**
 * Function: initMap
 * Usage:
 * ------
 */

void initMap(GameMap *gmap, Blitter *blitter);

/**
 * Function: updateMatch
 * Usage:
 * --------
 */

void updateMatch(GameMap *gmap, Blitter *blitter, Camera *camera, Timer *timer, xypair boxDest, xypair boxDimens, Uint32 boxColor);

/**
 * Function: updateBox
 * Usage: updateBox(&boxDest, &boxDimens, curr, held);
 * ---------------------------------------------------
 * Updates the selection box to match more recent dimensions.
 */

void updateBox(xypair *boxDest, xypair *boxDimens, xypair cursorCurr, xypair leftHeldDown);

/**
 * Function: handleMouseUp/Down
 * Usage: handleMouseUp(&event, &myData, &leftHeldDown);
 *        handleMouseDown(&event, &myData, &leftHeldDown);
 * --------------------------------------------------------------
 * Performs actions based on clicks. If the player clicks and releases
 * a unit with the left button, then that unit is selected. Alternatively,
 * the player can drag the left mouse button to form a selection box.
 * After selecting a unit or multiple units, the player may deselect by
 * left clicking somewhere else, or make a move command by right clicking
 * on a pathable tile. Right clicking on an enemy unit will initiate combat.
 */

void handleMouseUp(SDL_Event *event, GameMap *gmap, Camera *camera, xypair *leftHeldDown, bool *unitSelected, int *currentCommand);

void handleMouseDown(SDL_Event *event, GameMap *gmap, Camera *camera, xypair *leftHeldDown, bool *unitSelected, int *currentCommand);

/**
 * Function: handleKeyboardEvent
 * Usage:
 * --------
 */

void handleKeyboardEvent(Timer *timer, xypair *leftHeldDown, bool *unitSelected, int *currentCommand);

/**
 * Function: grabUnitGroup
 * Usage: toBeSelected = grabUnitGroup(released, held, mdat);
 * ----------------------------------------------------------
 * Returns a vector containing the locations of all units within the
 * boundaries of corner and opposite.
 */

std::vector<xypair> grabUnitGroup(xypair corner, xypair opposite, GameMap *gmap);

/**
 * Function: selectUnits
 * Usage: selectUnits(toBeSelected, mdat);
 * ---------------------------------------
 * Flags all units in toBeSelected as selected. They will draw circles
 * to indicate selection and will respond to commands.
 */

void selectUnits(std::vector<xypair> toBeSelected, GameMap *gmap);

void clearScreen(Blitter *blitter, SDL_Window *window);

/**
 * Function: getOffsets
 * Usage: offsets = getOffsets(x, y);
 * ----------------------------------
 * Interprets the (x, y) coordinates provided and returns a pair of
 * offsets for updating the camera. The coordinates should be of the
 * cursors current position on the screen.
 */

xypair getOffsets(int x, int y);

/**
 * Function: convertActualToMapLoc
 * Usage: xypair released = convertActualToMapLoc(x, y);
 * -----------------------------------------------------
 * Takes a specific (x,y) coordinate that should come from the
 * coordinates of a mouse click, and does some math to convert that
 * into the position those values correspond to on the game map.
 * For example, since the tiles on the map are 32x32, if the player
 * clicked the pixel at position (40, 40), this function would
 * convert that to (2, 2).
 */

xypair convertActualToMapLoc(int x, int y);

/* filenames */
#define MAPNAME "data/structureTest.txt"
#define SPRITES "data/allsprites.bmp"

/* Window resolution */
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

/* Window title */
#define WINDOW_TITLE "Project Ugly Goblin"

enum screenStates {MAINMENU, LOADING, MATCH, QUIT};

enum unitCommands {NONE, ATTACK, MOVE};

const Uint8 *KEYSTATE = SDL_GetKeyboardState(NULL);

SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color WHITE = {255, 255, 255, 255};

int main(int argc, char* args[])
{
    SDL_Window *window = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL2 could not initialize! SDL2_Error: %s\n", SDL_GetError();
    }
    else if(TTF_Init() < 0)
    {
        std::cout << "TTF could not initialize! Error: %s\n", SDL_GetError();
    }
    else
    {
        window = SDL_CreateWindow(
                     WINDOW_TITLE,
                     7,
                     28,
                     WINDOW_WIDTH,
                     WINDOW_HEIGHT,
                     SDL_WINDOW_SHOWN
                     | SDL_WINDOW_INPUT_GRABBED
                     // | SDL_WINDOW_FULLSCREEN_DESKTOP
                     );

        SDL_Surface *screen = NULL;
        screen = SDL_GetWindowSurface(window);
        TTF_Font *font = TTF_OpenFont("data/FreeSans.ttf", 26);
        if (font == NULL)
        {
            std::cout << SDL_GetError() << std::endl;
        }

        SDL_Surface *image = NULL;
        image = SDL_LoadBMP(SPRITES); // TODO: add error checking in case the file name is wrong

        spriteSrcLoc bitmapLocs[6];
        initSpriteLocs(bitmapLocs, 6);

        SDL_Surface *buffer = NULL;

        Blitter blitter(bitmapLocs, image, buffer, screen, font, WINDOW_WIDTH, WINDOW_HEIGHT);

        blitter.setTransparentColor(SDL_TRUE, 255, 0, 255); 
        
        GameMap gmap;
        Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, 10);
        Timer timer(WINDOW_WIDTH, WINDOW_HEIGHT);


        int screenState = MAINMENU;
        bool running = true;


        while(running)
        {
            switch (screenState)
            {
                case MAINMENU:
                    screenState = mainMenuLoop(&blitter, window);
                    break;
                case LOADING:
                    screenState = loadMatch(&blitter, &gmap, window);
                    break;
                case MATCH:
                    screenState = matchLoop(&gmap, &camera, &blitter, &timer, window);
                    break;
                case QUIT:
                    running = false;
                    break;
                default:
                    #ifdef PRINT_INFO
                    std::cout << "Failed to switch to proper game state." << std::endl;
                    #endif // PRINT_INFO
                    break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void initSpriteLocs(spriteSrcLoc *locsArray, int numSprites)
{
    int i = 0;
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            setSrcLoc(i, 32 * col, 32 * row, 32, 32, locsArray);
            i++;
            if (i >= numSprites)
            {
                return;
            }
        }
    }
}

void setSrcLoc(int spriteID, int x, int y, int h, int w, spriteSrcLoc *locsArray)
{
    #ifdef PRINT_INFO
    std::cout << "Initializing sprite ID " << spriteID << "." << std::endl;
    #endif // PRINT_INFO
    spriteSrcLoc src;
    src.x = x;
    src.y = y;
    src.h = h;
    src.w = w;
    locsArray[spriteID] = src;
}

int mainMenuLoop(Blitter *blitter, SDL_Window *window)
{
    blitter->blitText("Press Return to Start Game", WHITE, 200, 200, 200, 200, false);

    SDL_Event event;
    while (true)
    {
        SDL_UpdateWindowSurface(window);

        while(SDL_PollEvent(&event) != 0)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);

            if (KEYSTATE[SDL_SCANCODE_RETURN] || KEYSTATE[SDL_SCANCODE_RETURN2])
            {
                #ifdef PRINT_INFO
                std::cout << "return pressed, now loading" << std::endl;
                #endif // PRINT_INFO
                clearScreen(blitter, window);
                return LOADING;
            }
            if(event.type == SDL_QUIT || KEYSTATE[SDL_SCANCODE_ESCAPE])
            {
                #ifdef PRINT_INFO
                std::cout << "return pressed, now loading" << std::endl;
                #endif // PRINT_INFO
                clearScreen(blitter, window);
                return QUIT;
            }
        }
    }
}

int loadMatch(Blitter *blitter, GameMap *gmap, SDL_Window *window)
{
    blitter->blitText("Loading Match", WHITE, 200, 200, 200, 200, false);
    SDL_UpdateWindowSurface(window);
    initMap(gmap, blitter);
    clearScreen(blitter, window);
    return MATCH;
}

int matchLoop(GameMap *gmap, Camera *camera, Blitter *blitter, Timer *timer, SDL_Window *window)
{
    xypair camOffsets = {0, 0};
    xypair leftHeldDown = {-1, -1};
    xypair boxDest = {0, 0};
    xypair boxDimens = {0, 0};
    Uint32 boxColor = SDL_MapRGB(blitter->getFormat(),  0, 255, 0);
    bool matchInProgress = true;
    bool unitSelected = false;
    int currentCommand = NONE;
    SDL_Event event;
    timer->start();

    while (matchInProgress)
    {
        updateMatch(gmap, blitter, camera, timer, boxDest, boxDimens, boxColor);
        camera->slide(camOffsets.x, camOffsets.y, gmap->getWidth(), gmap->getHeight());
        SDL_UpdateWindowSurface(window);

        while(SDL_PollEvent(&event) != 0)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            camOffsets = getOffsets(x, y);

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                handleMouseDown(&event, gmap, camera, &leftHeldDown, &unitSelected, &currentCommand);
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                handleMouseUp(&event, gmap, camera, &leftHeldDown, &unitSelected, &currentCommand);
                boxDest = {0, 0};
                boxDimens = {0, 0};
            }
            else
            {
                if ((x != leftHeldDown.x && y != leftHeldDown.y) && (leftHeldDown.x >= 0 && leftHeldDown.y >= 0))
                {
                    xypair curr = {x + camera->getXOffset(), y + camera->getYOffset()};
                    xypair held = leftHeldDown;
                    updateBox(&boxDest, &boxDimens, curr, held);
                }
            }
            if (event.type == SDL_KEYDOWN)
            {
                handleKeyboardEvent(timer, &leftHeldDown, &unitSelected, &currentCommand);
            }

            if(event.type == SDL_QUIT || KEYSTATE[SDL_SCANCODE_ESCAPE])
            {
                matchInProgress = false;
            }
        }
    }
    return QUIT;
}

void initMap(GameMap *gmap, Blitter *blitter)
{
    //string mapFilename;
    //cout << "Enter map filename: ";
    //cin >> mapFilename;
    //cout << endl;
    //mapFilename = "data/" + mapFilename;
    while (gmap->loadMap(MAPNAME, blitter) == false)
    {
        return;
        //cout << "Try again: ";
        //cin >> mapFilename;
        //cout << endl;
        //mapFilename = "data/" + mapFilename;
    }
}

void updateMatch(GameMap *gmap, Blitter *blitter, Camera *camera, Timer *timer, xypair boxDest, xypair boxDimens, Uint32 boxColor)
{
    gmap->updateMap(blitter, timer);
    blitter->blitFilledRect(boxDest, boxDimens, boxColor, 1, true);
    blitter->blitBuffToScreen(camera->getXOffset(), camera->getYOffset());
    timer->blit(blitter);
}

void updateBox(xypair *boxDest, xypair *boxDimens, xypair cursorCurr, xypair leftHeldDown)
{
    xypair minPair;
    minPair = getMinPair(cursorCurr, leftHeldDown);
    *boxDest = {minPair.x, minPair.y};
    *boxDimens = {abs(cursorCurr.x - leftHeldDown.x), abs(cursorCurr.y - leftHeldDown.y)};
}

void handleMouseUp(SDL_Event *event, GameMap *gmap, Camera *camera, xypair *leftHeldDown, bool *unitSelected, int *currentCommand)
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {
        if (*currentCommand == NONE)
        {
            int x = event->button.x + (camera->getXOffset());
            int y = event->button.y + (camera->getYOffset());
            xypair released = convertActualToMapLoc(x, y);
            xypair held = convertActualToMapLoc(leftHeldDown->x, leftHeldDown->y);
            std::vector<xypair> toBeSelected;
            if (released.x == held.x && released.y == held.y)
            {
                if (gmap->checkUnit(held.x, held.y))
                {
                    toBeSelected.push_back(held);
                }
            }
            else
            {
                toBeSelected = grabUnitGroup(released, held, gmap);
            }
            *leftHeldDown = {-1, -1};
            if(toBeSelected.size() > 0)
            {
                if (*unitSelected == true && (KEYSTATE[SDL_SCANCODE_LSHIFT] || KEYSTATE[SDL_SCANCODE_RSHIFT]))
                {
                    selectUnits(toBeSelected, gmap);
                }
                else if (*unitSelected == false)
                {
                    *unitSelected = true;
                    gmap->deselect(-1, -1);
                    selectUnits(toBeSelected, gmap);
                }
                else
                {
                    *unitSelected = true;
                    gmap->deselect(-1, -1);
                    selectUnits(toBeSelected, gmap);
                }
            }
            else
            {
                *unitSelected = false;
                gmap->deselect(-1, -1);
            }
        }
        else
        {
            #ifdef PRINT_INFO
            std::cout << "setting current command to none" << std::endl;
            #endif // PRINT_INFO
            *currentCommand = NONE;
        }
    }

    if (event->button.button == SDL_BUTTON_RIGHT)
    {
        if(*unitSelected == true)
        {
            int x = event->button.x + (camera->getXOffset());
            int y = event->button.y + (camera->getYOffset());

            xypair mapLoc = convertActualToMapLoc(x, y);
            gmap->findPath(-1, -1, mapLoc.x, mapLoc.y);
        }
    }
}

void handleMouseDown(SDL_Event *event, GameMap *gmap, Camera *camera, xypair *leftHeldDown, bool *unitSelected, int *currentCommand)
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {
        if (*unitSelected == true && *currentCommand != NONE)
        {
            if (*currentCommand == ATTACK)
            {
                gmap->findTarget(-1, -1);
            }
            if (*currentCommand == MOVE)
            {
                int x = event->button.x + (camera->getXOffset());
                int y = event->button.y + (camera->getYOffset());

                xypair mapLoc = convertActualToMapLoc(x, y);
                gmap->findPath(-1, -1, mapLoc.x, mapLoc.y);
            }
        }
        else
        {
            leftHeldDown->x = event->button.x + (camera->getXOffset());
            leftHeldDown->y = event->button.y + (camera->getYOffset());
        }
    }

    if (event->button.button == SDL_BUTTON_RIGHT)
    {
        leftHeldDown->x = -1;
        leftHeldDown->y = -1;
    }
}

void handleKeyboardEvent(Timer *timer, xypair *leftHeldDown, bool *unitSelected, int *currentCommand)
{
    if (KEYSTATE[SDL_SCANCODE_P])
    {
        if (timer->checkPause())
        {
            timer->resume();
        }
        else
        {
            timer->pause();
        }

        #ifdef PRINT_INFO
        std::cout << "GameLoop -> handleKeyboardEvent: key press paused or unpaused the timer" << std::endl;
        #endif // PRINT_INFO
    }
    if (KEYSTATE[SDL_SCANCODE_A])
    {
        if (*unitSelected)
        {
            #ifdef PRINT_INFO
            std::cout << "current command set to attack" << std::endl;
            #endif // PRINT_INFO
            *currentCommand = ATTACK;
        }
    }
    if (KEYSTATE[SDL_SCANCODE_M])
    {
        if (*unitSelected)
        {
            #ifdef PRINT_INFO
            std::cout << "current command set to move" << std::endl;
            #endif // PRINT_INFO
            *currentCommand = MOVE;
        }
    }
}

std::vector<xypair> grabUnitGroup(xypair corner, xypair opposite, GameMap *gmap)
{
    std::vector<xypair> selected;
    xypair minPair = getMinPair(corner, opposite);
    xypair maxPair = getMaxPair(corner, opposite);

    #ifdef PRINT_INFO
    std::cout << "GameLoop -> grabUnitGroup: minPair: (" << minPair.x << "," << minPair.y << ")" << std::endl;
    std::cout << "GameLoop -> grabUnitGroup: maxPair: (" << maxPair.x << "," << maxPair.y << ")" << std::endl;
    #endif // PRINT_INFO

    for (int x = minPair.x; x <= maxPair.x; x++)
    {
        for (int y = minPair.y; y <= maxPair.y; y++)
        {
            if (gmap->checkUnit(x, y))
            {
                xypair selectedUnit = {x, y};
                selected.push_back(selectedUnit);
            }
        }
    }
    return selected;
}

void selectUnits(std::vector<xypair> toBeSelected, GameMap *gmap)
{
    for (int i = 0; i < toBeSelected.size(); i++)
    {
        #ifdef PRINT_INFO
        std::cout << "GameLoop -> selectUnits: selecting: (" << toBeSelected[i].x << "," << toBeSelected[i].y << ")" << std::endl;
        #endif // PRINT_INFO

        gmap->select(toBeSelected[i].x, toBeSelected[i].y); // TODO pass in entire vector instead?
    }
}

void clearScreen(Blitter *blitter, SDL_Window *window)
{
    blitter->clearScreen();
    SDL_UpdateWindowSurface(window);
}

xypair getOffsets(int x, int y)
{
    xypair offsets;
    if (x >= WINDOW_WIDTH - 10) // scroll right
    {
        offsets.x = 1;
    }
    if (x <= 10) // left
    {
        offsets.x = -1;
    }
    if (x < WINDOW_WIDTH - 10 && x > 10) // stop scroll
    {
        offsets.x = 0;
    }
    if (y >= WINDOW_HEIGHT - 10) // down
    {
        offsets.y = 1;
    }
    if (y <= 10) // up
    {
        offsets.y = -1;
    }
    if (y < WINDOW_HEIGHT - 10 && y > 10) // stop
    {
        offsets.y = 0;
    }
    return offsets;
}

xypair convertActualToMapLoc(int x, int y)
{
    float xf = x;
    float yf = y;

    xf = xf / 32;
    yf = yf / 32;

    xf = truncf(xf);
    yf = truncf(yf);

    xypair mapLoc;
    mapLoc.x = xf;
    mapLoc.y = yf;

    return mapLoc;
}


