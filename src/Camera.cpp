#include "Camera.h"

Camera::Camera()
{
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.w = 0;
    this->offset.h = 0;
    this->scrollSpeed = 0;
}

Camera::Camera(int windowWidth, int windowHeight, int scrollSpeed)
{
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.w = windowWidth;
    this->offset.h = windowHeight;
    this->scrollSpeed = scrollSpeed;
}

int Camera::getXOffset()
{
    return this->offset.x;
}

int Camera::getYOffset()
{
    return this->offset.y;
}

void Camera::slide(int xSlide, int ySlide, int mapWidth, int mapHeight)
{
    xSlide *= this->scrollSpeed;
    ySlide *= this->scrollSpeed;

    // want to check if new location will be outside of borders
    // before setting the new value
    int temp = this->offset.x + xSlide;
    if (!(temp < 0 || temp + this->offset.w > (mapWidth * 32)))
    {
        this->offset.x = temp;
    }

    temp = this->offset.y + ySlide;
    if (!(temp < 0 || temp + this->offset.h > (mapHeight * 32)))
    {
        this->offset.y = temp;
    }
}

Camera::~Camera()
{
    //dtor
}
