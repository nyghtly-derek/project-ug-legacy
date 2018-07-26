
#ifndef CAMERA_H
#define CAMERA_H

#include "Additional.h"

/**
 * Class: Camera
 * -------------
 * Manages the location of the Camera, which determines what portion
 * of the map will be shown.
 *
 * Methods: getXOffset, getYOffset, slide
 */

class Camera
{
    public:
        Camera();
        Camera(int windowWidth, int windowHeight, int scrollSpeed);

/**
 * Method: getXOffset
 * Usage in GameLoop:
 *  xypair curr = {x + camera->getXOffset(), y + camera->getYOffset()};
 * ---------------------------------------------
 * Returns the x value of the top-left point of the camera.
 */

        int getXOffset();

/**
 * Method: getYOffset
 * Usage in GameLoop:
 *  xypair curr = {x + camera->getXOffset(), y + camera->getYOffset()};
 * ---------------------------------------------
 * Returns the y value of the top-left point of the camera.
 */

        int getYOffset();

/**
 * Method: slide
 * Usage in GameLoop:
 *  camera->slide(camOffsets.x, camOffsets.y, gmap->getWidth(), gmap->getHeight());
 * ------------------------------------------------------------
 * Changes the (x,y) location of the camera. The camera determines
 * which chunk of the map will be clipped to the screen. The numbers
 * passed into xSlide and ySlide determine how many pixels the camera
 * will shift. This number is scaled by the camera's scrollSpeed.
 */

        void slide(int xSlide, int ySlide, int mapWidth, int mapHeight);
        ~Camera();
    protected:
    private:
        SDL_Rect offset;
        int scrollSpeed;
};

#endif // CAMERA_H
