#include "MouseTargetMarker.h"

MouseTargetMarker::MouseTargetMarker()
{
    x = 0;
    y = 0;
    radius = 0.0f;
    maxRadius = 0.0f;
    time = 0.0f;
    fadeTime = 0.0f;
    isFinished = true;
}

MouseTargetMarker::~MouseTargetMarker()
{
    //dtor
}

void MouseTargetMarker::draw(sf::RenderWindow& window)
{
    if(!isFinished)
    {
        sf::CircleShape circle;
        circle.setPosition(x - radius, y - radius);
        circle.setRadius(radius);
        //circle.setOrigin(maxRadius/2.0f, maxRadius/2.0f);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(3.0f);

        window.draw(circle);
    }
}

void MouseTargetMarker::update()
{
    if(!isFinished)
    {
        if(time < fadeTime)
        {
            time++;

            float percTime = time/fadeTime;

            radius = maxRadius * percTime;
            color = sf::Color(color.r, color.g, color.b, 255 - 255 * percTime);
        }
        else
        {
            isFinished = true;
            time = 0.0f;
        }
    }

}

void MouseTargetMarker::createCircle(int x, int y, float fadeTime, float maxRadius, sf::Color color)
{
    this->x = x;
    this->y = y;
    this->fadeTime = fadeTime * FRAMES_PER_SECOND;
    this->maxRadius = maxRadius;
    this->color = color;
    isFinished = false;
    time = 0.0f;
}
