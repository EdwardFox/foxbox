#ifndef MOUSETARGETMARKER_H
#define MOUSETARGETMARKER_H

#include "utility.h"

class MouseTargetMarker
{
    public:
        MouseTargetMarker();
        virtual ~MouseTargetMarker();

        void draw(sf::RenderWindow& window);
        void update();

        void createCircle(int x, int y, float fadeTime, float maxRadius, sf::Color color);
    protected:
        int x;
        int y;

        float radius;
        float maxRadius;
        float time;
        float fadeTime;

        sf::Color color;

        bool isFinished;
    private:
};

#endif // MOUSETARGETMARKER_H
