#ifndef TARGETAREA_H
#define TARGETAREA_H

#include <SFML/Graphics.hpp>
#include "utility.h"

class TargetArea
{
    public:
        TargetArea();
        TargetArea(float x, float y);
        TargetArea(float x, float y, float r);
        virtual ~TargetArea();

        sf::Vector2f position;
        sf::Vector2f originalPosition;
        float radius;
    protected:
    private:
};

#endif // TARGETAREA_H
