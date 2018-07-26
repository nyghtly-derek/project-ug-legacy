
#ifndef TIMER_H
#define TIMER_H

#include "Additional.h"
#include "Blitter.h"

/**
 * Class: Timer
 * ------------
 * This class keeps track of time for a match.
 *
 * Methods: start, pause, resume, checkPause, getTime, blit
 */

class Timer
{
    public:
        Timer();
        Timer(int windowWidth, int windowHeight);

/**
 * Method: start
 * Usage in GameLoop: timer->start();
 * ----------------------------------
 * Starts the timer.
 */

        void start();

/**
 * Method: pause
 * Usage in GameLoop: timer->pause();
 * -------------------------------------------------------
 * Pauses the timer. If the timer is paused, does nothing.
 */

        void pause();

/**
 * Method: resume
 * Usage in GameLoop: timer->resume();
 * --------------------------------------------------------
 * Resumes the timer. If the timer is running, does nothing.
 */

        void resume();

/**
 * Method: checkPause
 * Usage in GameLoop: if (timer->checkPause())...
 * ----------------------------------------------
 * Returns true if paused, false if not.
 */

        bool checkPause();

/**
 * Method: getTime
 * Usage in UnitManager: current->giveAttack(timer->getTime());
 * ------------------------------------------------------------
 * Returns the current time in milliseconds.
 */

        int getTime();

/**
 * Method: blit
 * Usage in GameLoop: timer->blit(blitter);
 * -----------------------------------------------------------------
 * Draws the current time on screen, and a message if game is paused.
 */

        void blit(Blitter *blitter);

        ~Timer();

    private:
        int windowWidth;
        int windowHeight;
        int startTime;
        int pauseTime;
        bool paused;
        SDL_Color color;

/**
 * Private Method: getTimeConvert
 * ------------------------------
 * Returns the current time converted to an hr:min:sec format using the
 * timeKeep struct.
 */

        timeKeep getTimeConvert();
};

#endif // TIMER_H
