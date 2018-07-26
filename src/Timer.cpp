#include "Timer.h"

Timer::Timer()
{
    this->startTime = 0;
    this->pauseTime = 0;
    this->paused = true;
    this->color.r = 255;
    this->color.g = 255;
    this->color.b = 255;
}

Timer::Timer(int windowWidth, int windowHeight)
{
    this->startTime = 0;
    this->pauseTime = 0;
    this->paused = true;
    this->color.r = 255;
    this->color.g = 255;
    this->color.b = 255;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

void Timer::start()
{
    this->startTime = SDL_GetTicks();
}

void Timer::pause()
{
    if (!this->paused)
    {
        this->pauseTime = SDL_GetTicks();
        this->paused = true;
    }
}

bool Timer::checkPause()
{
    return this->paused;
}

void Timer::resume()
{
    if (this->paused)
    {
        this->startTime = this->startTime + (SDL_GetTicks() - this->pauseTime);
        this->pauseTime = 0;
        this->paused = false;
    }
}

int Timer::getTime()
{
    if (this->paused)
    {
        return pauseTime - this->startTime;
    }
    else
    {
        return SDL_GetTicks() - this->startTime;
    }
}

void Timer::blit(Blitter *blitter)
{
    timeKeep viewTime = this->getTimeConvert();

    std::stringstream str;

    str << viewTime.hours << ":";
    str.fill('0');
    str.width(2);
    str << viewTime.minutes << ":";
    str.fill('0');
    str.width(2);
    str << viewTime.seconds;
    std::string timeText;
    str >> timeText;

    const char *charText = timeText.c_str();

    blitter->blitText(charText, this->color, 0, 0, 1200, 1200, false);

    if (this->checkPause())
    {
        const char *charText = "PAUSED";

        blitter->blitText(charText, this->color,
                          this->windowWidth / 2 - 50,
                          this->windowHeight / 2 - 20,
                          1200, 1200, false);

        charText = "PRESS P TO RESUME";

        blitter->blitText(charText, this->color,
                          this->windowWidth / 2 - 120,
                          this->windowHeight / 2 + 10,
                          600, 600, false);
    }

}

Timer::~Timer()
{}

/*
 * Private Methods
 * ---------------
 */

timeKeep Timer::getTimeConvert()
{
    timeKeep viewTime;
    int milliTime = this->getTime();
    viewTime.seconds = (milliTime / 1000) % 60;
    viewTime.minutes = (milliTime / (1000 * 60)) % 60;
    viewTime.hours = (milliTime / (1000 * 60 * 60)) % 24;
    return viewTime;
}

