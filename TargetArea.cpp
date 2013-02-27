#include "TargetArea.h"

TargetArea::TargetArea()
{
    radius = 2.0f * BOX_SIZE;
}

TargetArea::TargetArea(float x, float y)
{
    position.x = x;
    position.y = y;
    originalPosition = position;
    radius = 1 * BOX_SIZE;
}

TargetArea::TargetArea(float x, float y, float r)
{
    position.x = x;
    position.y = y;
    originalPosition = position;
    radius = r;
}

TargetArea::~TargetArea()
{
    //dtor
}
