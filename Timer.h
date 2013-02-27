#ifndef TIMER_H
#define TIMER_H

#include "utility.h"


class Timer
{
    public:
        Timer();
        virtual ~Timer();

        void setThreshold(float t);
        void update();
        void resetTimer();
        void endTimer();

        bool timeReached();

        float getCurrentTime();
        float getTimeLeft();
        float getThreshold();
    protected:
        float timer;
        float threshold;
    private:
};

#endif // TIMER_H
