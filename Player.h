#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>

#include "utility.h"
#include "Box.h"
#include "Events.h"
#include "World.h"
#include "TargetArea.h"

// Forward declaration to prevent circular header dependencies
class World;

class Player
{
    public:
        Player();
        virtual ~Player();

        void update(b2World &physicsWorld, Events gameEvents, World &gameWorld);
        void draw(sf::RenderWindow& window);
        b2Vec2 getNewFocus();

        bool hasNewFocus;
        std::vector<int> selectedBoxes;

        int getResources();
        void addResources(int amount);
    protected:
    private:
        void clearActiveBoxes(World& gameWorld);
        void calcSelectionArea(Events gameEvents);
        void setSelectionArea(Events gameEvents);
        void setPushVisuals(Events gameEvents);
        void forcePushBox(int mouseX, int mouseY);
        void setBoxTransformVisuals(Events gameEvents);
        void transformBoxToAlly();

        sf::RectangleShape selectionArea;
        sf::Vector2f selectionAreaPosition;
        sf::Vector2f selectionAreaSize;

        sf::Vertex line[2];
        sf::Text resourceConsumption;

        float forcePushPower;

        bool selectionAreaActive;
        bool pushActive;
        bool transformBoxActive;

        Box* pushBox;
        Box* transformBox;

        int transformCost;
        int resourceAmount;

        b2Vec2 newFocus;
};

#endif // PLAYER_H
