#include "Blitter.h"

Blitter::Blitter(spriteSrcLoc *locsArray, SDL_Surface *bitmap,
                SDL_Surface *buffer, SDL_Surface *screen,
                TTF_Font *font, int windowWidth, int windowHeight)
{
    this->locsArray = locsArray;
    this->bitmap = bitmap;
    this->buffer = buffer;
    this->screen = screen;
    this->font = font;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

void Blitter::createBuffer(Uint32 flags, int width, int height, int depth,
                       Uint32 rMask, Uint32 gMask, Uint32 bMask, Uint32 aMask)
{
    this->buffer = SDL_CreateRGBSurface(flags, width, height, depth, rMask, gMask, bMask, aMask);
}

void Blitter::setTransparentColor(int flag, int r, int g, int b)
{
    SDL_SetColorKey(this->bitmap, flag, SDL_MapRGB(this->bitmap->format, r, g, b));
}

SDL_PixelFormat * Blitter::getFormat()
{
    return this->screen->format;
}

void Blitter::blitSprite(int spriteID, int destX, int destY, bool toBuffer)
{
    spriteSrcLoc loc = this->locsArray[spriteID];

    if (toBuffer)
    {
        this->blitSurface(this->bitmap, this->buffer, loc.x, loc.y, loc.w, loc.h,
                          destX, destY, loc.w, loc.h);
    }
    else
    {
        this->blitSurface(this->bitmap, this->screen, loc.x, loc.y, loc.w, loc.h,
                          destX, destY, loc.w, loc.h);
    }
}

void Blitter::blitBuffToScreen(int cameraX, int cameraY)
{
    this->blitSurface(this->buffer, this->screen, 0 + cameraX, 0 + cameraY,
                    this->windowWidth, this->windowHeight, 0, 0,
                    this->windowWidth, this->windowHeight);
}

void Blitter::blitText(const char *text, SDL_Color color, int destX, int destY,
                       int destW, int destH, bool toBuffer)
{
    SDL_Surface *message = TTF_RenderText_Solid(this->font, text, color);

    if (toBuffer)
    {
        this->blitSurface(message, this->buffer, -1, -1, -1, -1,
                      destX, destY, destW, destH);
    }
    else
    {
        this->blitSurface(message, this->screen, -1, -1, -1, -1,
                      destX, destY, destW, destH);
    }
}

void Blitter::drawLine(int x0, int y0, int x1, int y1, Uint32 color, bool toBuffer)
{
    if (x0 > x1)
    {
        int buff = x0;
        x0 = x1;
        x1 = buff;
    }
    if (y0 > y1)
    {
        int buff = y0;
        y0 = y1;
        y1 = buff;
    }

    if (x0 == x1) // if vertical line
    {
        for (int y = y0; y < y1; y++)
        {
            this->setPixel(x0, y, color, toBuffer);
        }
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2 * dy - dx;
    this->setPixel(x0, y0, color, toBuffer);
    int y = y0;

    for ( int x = x0 + 1; x < x1; x++)
    {
        if (D > 0)
        {
           y++;
           this->setPixel(x, y, color, toBuffer);
           D = D + (2*dy - 2*dx);
        }
        else
        {
            this->setPixel(x, y, color, toBuffer);
            D = D + (2*dy);
        }
    }
}

void Blitter::drawEmptyRect(xypair position, xypair dimension, Uint32 color, bool toBuffer)
{
    this->drawLine(position.x, position.y, position.x + dimension.x, position.y, color, toBuffer); // top line
    this->drawLine(position.x, position.y, position.x, position.y + dimension.y, color, toBuffer); // left line
    this->drawLine(position.x + dimension.x, position.y, position.x + dimension.x, position.y + dimension.y, color, toBuffer); // right line
    this->drawLine(position.x, position.y + dimension.y, position.x + dimension.x, position.y + dimension.y, color, toBuffer); //bottom line
}

void Blitter::blitFilledRect(xypair dest, xypair dimension, Uint32 rgb, int alpha, bool toBuffer)
{
    SDL_Surface *s = NULL;
    s = SDL_CreateRGBSurface(0, dimension.x, dimension.y, 32, 0, 0, 0, alpha);
    SDL_FillRect(s, NULL, rgb);
    SDL_SetSurfaceAlphaMod(s, 0x44);
    SDL_Rect destRect = {dest.x, dest.y, dimension.x, dimension.y};

    if (toBuffer)
    {
        this->trueBlit(s, NULL, this->buffer, &destRect);
    }
    else
    {
        this->trueBlit(s, NULL, this->screen, &destRect);
    }
}

void Blitter::clearScreen()
{
    SDL_FillRect(this->screen, NULL, SDL_MapRGB(this->getFormat(), 0, 0, 0));
}

Blitter::~Blitter()
{
    SDL_FreeSurface(this->bitmap);
}

/*
 * Private Methods
 * ---------------
 */

void Blitter::setPixel(int x, int y, Uint32 color, bool toBuffer)
{
    if (toBuffer)
    {
        Uint8 *targetPixel = (Uint8 *)this->buffer->pixels + y * this->buffer->pitch + x * 4;
        *(Uint32 *)targetPixel = color;
    }
    else
    {
        Uint8 *targetPixel = (Uint8 *)this->screen->pixels + y * this->screen->pitch + x * 4;
        *(Uint32 *)targetPixel = color;
    }
}

void Blitter::blitSurface(SDL_Surface *source, SDL_Surface *destination,
                          int srcX, int srcY, int srcW, int srcH,
                          int destX, int destY, int destW, int destH)
{
    SDL_Rect sourceRect;
    sourceRect.x = srcX;
    sourceRect.y = srcY;
    sourceRect.w = srcW;
    sourceRect.h = srcH;

    SDL_Rect destinationRect;
    destinationRect.x = destX;
    destinationRect.y = destY;
    destinationRect.w = destW;
    destinationRect.h = destH;

    if (srcX < 0 && srcY < 0 && srcW < 0 && srcH < 0)
    {
        this->trueBlit(source, NULL, destination, &destinationRect);
    }
    else if (destX < 0 && destY < 0 && destW < 0 && destH < 0)
    {
        this->trueBlit(source, &sourceRect, destination, NULL);
    }
    else
    {
        this->trueBlit(source, &sourceRect, destination, &destinationRect);
    }
}

void Blitter::trueBlit(SDL_Surface *source, SDL_Rect *srcRect, SDL_Surface *destination, SDL_Rect *destRect)
{
    int error = 0;
    error = SDL_BlitSurface(source, srcRect, destination, destRect);
    if (error != 0)
    {
        std::cout << "An error with SDL has occurred: " << SDL_GetError() << std::endl;
    }
}
