#ifndef GIBS_H
#define GIBS_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "utility.h"
#include "Timer.h"

class Gibs
{
    public:
        Gibs();
        virtual ~Gibs();

        void update();
        void draw(sf::RenderWindow& window);
        void createGib(b2World& world, b2Vec2 position, b2Vec2 size, sf::Color color, uint16 categoryBits, uint16 maskBits);
        void activateFloatGibs();

        bool isDead;

        b2Body* getBody();
    protected:
        void setRandomForceToCenter();

        b2Body* body;
        sf::Color color;
        sf::RectangleShape rect;
        Timer fadeTimer;
    private:
};

#endif // GIBS_H
