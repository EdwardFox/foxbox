#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include "Events.h"
#include "utility.h"


class Minimap
{
    public:
        // ------------------------------- Methods -------------------------------
        Minimap();
        virtual ~Minimap();

        void setSize(int width, int height, int wWidth, int wHeight);
        void calcViewport(float size);
        void setCameraPosition(int x, int y);

        sf::View getUpdatedView();

        void update(Events& gameEvents);
        void draw(sf::RenderWindow& window);
        void drawCameraWindow(sf::RenderWindow& window);

        // ------------------------------- Attributes -------------------------------
        sf::Vector2f newViewCenter;
        bool updateViewThroughMinimap;
    protected:
        // ------------------------------- Methods -------------------------------
        void calcCenter();
        void processClicks(Events& gameEvents);

        // ------------------------------- Attributes -------------------------------
        int width;
        int height;
        int centerX;
        int centerY;

        int windowWidth;
        int windowHeight;

        float scale;
        float viewportX;
        float viewportY;
        float viewportWidth;
        float viewportHeight;
        float outLineThickness;

        float widthMod;
        float heightMod;

        sf::View minimap;

        sf::RectangleShape background;
        sf::RectangleShape camera;
    private:
};

#endif // MINIMAP_H
