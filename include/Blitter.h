
#ifndef BLITTER_H
#define BLITTER_H

#include "Additional.h"

/**
 * Class: Blitter
 * --------------
 * Draws and blits things to the screen including: text, sprites,
 * and lines.
 *
 * Methods: createBuffer, setTransparentColor, getFormat, blitSprite,
 *          blitBuffToScreen, blitText, drawLine, drawEmptyRect,
 *          drawFilledRect, clearScreen
 */

class Blitter
{
    public:
        Blitter(spriteSrcLoc *locsArray, SDL_Surface *bitmap,
                SDL_Surface *buffer, SDL_Surface *screen,
                TTF_Font *font, int windowWidth, int windowHeight);

/**
 * Method: createBuffer
 * Usage in GameMap:
 *  this->blitter->createBuffer(0, width * 32, height * 32, 32, 0, 0, 0, 0xff000000);
 * ----------------------------------------------------------------------------------
 * Creates a buffer screen allowing the user to draw there first then blit
 * several layers at once to the main screen.
 */

        void createBuffer(Uint32 flags, int width, int height, int depth,
                       Uint32 rMask, Uint32 gMask, Uint32 bMask, Uint32 aMask);

/**
 * Method: setTransparentColor
 * Usage in GameLoop: blitter.setTransparentColor(SDL_TRUE, 255, 0, 255);
 * ----------------------------------------------------------------------
 * Allows the user to set colors as transparent or reverse by passing in
 * SDL_TRUE (for transparent) or SDL_FALSE (for not) and the target color.
 */

        void setTransparentColor(int flag, int r, int g, int b);

/**
 * Method: getFormat
 * Usage in GameLoop: Uint32 boxColor = SDL_MapRGB(blitter.getFormat(),  0, 255, 0);
 * ---------------------------------------------------------------------------------
 * Returns the format for screen, needed in some SDL functions.
 */

        SDL_PixelFormat *getFormat();

/**
 * Method: blitSprite
 * Usage in Unit: blitter->blitSprite(this->unitID, this->x * 32, this->y * 32, true);
 * -----------------------------------------------------------------------------------
 * Blits sprite of provided ID to provided destination. By default blits to the
 * screen directly but can be blit to buffer if toBuffer value is passed in as true.
 */

        void blitSprite(int spriteID, int destX, int destY, bool toBuffer);

/**
 * Method: blitBuffToScreen
 * Usage in MatchData:
 *  this->blitter->blitBuffToScreen(this->getCameraX(), this->getCameraY());
 * -------------------------------------------------------------------------
 * Blits the buffer to the screen, using the camera position values to decide
 * what to clip from the buffer (because it should be bigger than the screen
 * window).
 */

        void blitBuffToScreen(int cameraX, int cameraY);

/**
 * Method: blitText
 * Usage in Timer:
 *  blitter->blitText(this->font, charText, this->color, 0, 0, 1200, 1200, false);
 * -------------------------------------------------------------------------------
 * Blits provided text to the screen in provided color to provided destination. By
 * default blits to the screen directly but can be blit to buffer if toBuffer value
 * is passed in as true.
 */
        void blitText(const char *text, SDL_Color color, int destX, int destY,
                      int destW, int destH, bool toBuffer);

/**
 * Method: drawLine
 * Usage in Unit:
 * blitter->drawLine(this->x * 32 + 16, this->y * 32 + 16,
 *                   this->destination.x * 32 + 16,
 *                   this->destination.y * 32 + 16,
 *                   0x00FF00, true);
 * --------------------------------------------------------
 * Uses Bresenham's line algorithm and setPixel to draw from (x0,y0)
 * to (x1,y1) in provided color. By default blits to the screen directly,
 * but can be blit to buffer if toBuffer value is passed in as true.
 */

        void drawLine(int x0, int y0, int x1, int y1, Uint32 color, bool toBuffer);

/**
 * Method: drawEmptyRect
 * Usage in Unit:
 *  blitter->drawEmptyRect(healthBarDestination, healthBarDimension, 0x00FF00, true);
 * ----------------------------------------------------------------------------------
 * Uses drawLine to draw an empty rectangle with provided values. By default blits to
 * the screen directly but can be blit to buffer if toBuffer value is passed in as
 * true.
 */

        void drawEmptyRect(xypair position, xypair dimension, Uint32 color, bool toBuffer);

/**
 * Method: blitFilledRect
 * Usage in Unit:
 *  blitter->blitFilledRect(healthBarDestination, healthBarDimension, 0x00FF00, 0, true);
 * --------------------------------------------------------------------------------------
 * Uses SDL to draw a filled rectangle. By default blits to the screen directly but can
 * be blit to buffer if toBuffer value is passed in as true.
 */

        void blitFilledRect(xypair dest, xypair dimension, Uint32 rgb, int alpha, bool toBuffer);

/**
 * Method: clearScreen
 * Usage in GameMap: this->blitter->clearScreen();
 * -----------------------------------------------
 * Clears the screen for further drawing.
 */

        void clearScreen();
        virtual ~Blitter();
    protected:
    private:
        spriteSrcLoc *locsArray;
        SDL_Surface *bitmap;
        SDL_Surface *screen;
        SDL_Surface *buffer;
        TTF_Font *font;
        int windowWidth;
        int windowHeight;

/**
 * Private Method: setPixel
 * ------------------------
 * Sets pixel at (x,y) to provided color. Implementation borrowed
 * from: http://www.dreamincode.net/forums/topic/246895-draw-lines-in-c-with-sdl/
 */

        void setPixel(int x, int y, Uint32 color, bool toBuffer);

/**
 * Private Method: blitSurface
 * ---------------------------
 * Blits to screen using provided values. Passing numbers less than one for either
 * all the src values or all the dest values causes the entire surface to be used
 * rather than just a portion.
 */

        void blitSurface(SDL_Surface *source, SDL_Surface *destination,
                          int srcX, int srcY, int srcW, int srcH,
                          int destX, int destY, int destW, int destH);

/**
 * Private Method: trueBlit
 * ------------------------
 * Wrapper class for SDL's blit function, but has error checking included.
 */

        void trueBlit(SDL_Surface *source, SDL_Rect *srcRect,
                      SDL_Surface *destination, SDL_Rect *destRect);
};

#endif // BLITTER_H
