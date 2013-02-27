#include "Timer.h"

Timer::Timer()
{
    timer = 0.0f;
    threshold = 1 * FRAMES_PER_SECOND;
}

Timer::~Timer()
{
    //dtor
}

void Timer::setThreshold(float t)
{
    threshold = t * FRAMES_PER_SECOND;
}

bool Timer::timeReached()
{
    if(timer >= threshold)
    {
        return true;
    }

    return false;
}

void Timer::resetTimer()
{
   timer = 0;
}

void Timer::update()
{
    timer++;
}

float Timer::getCurrentTime()
{
    return (timer / FRAMES_PER_SECOND);
}

float Timer::getTimeLeft()
{
    return threshold / FRAMES_PER_SECOND - getCurrentTime();
}

void Timer::endTimer()
{
    timer = 2*threshold;
}

float Timer::getThreshold()
{
    return threshold / FRAMES_PER_SECOND;
}
