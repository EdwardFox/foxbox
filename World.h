#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "utility.h"
#include "BoomBox.h"
#include "ChargeBox.h"
#include "FloatBox.h"
#include "MagicBox.h"
#include "FoxBox.h"
#include "TargetArea.h"
#include "Player.h"
#include "Timer.h"
#include "Gibs.h"

// Forward declaration to prevent circular header dependencies
class Player;

class World
{
    public:
        World();
        virtual ~World();

        int getBoxIdByCoordinates(int x, int y);

        Box* getBoxById(int id);

        sf::Texture& getTexture(std::string texName);

        std::vector<Box*>* getBoxes();
        std::map<std::string, Box*>* getGroundBoxes();
        std::vector<int> getBoxesBySelection(sf::Vector2f position, sf::Vector2f size);
        std::vector<int> getSelectedBoxes();
        std::vector<int> selectAllDynamicBoxes();
        std::vector<int> getAllAlliesByType(std::string type);

        void generateWorld(b2World& physicsWorld);
        void regenerateWorld(b2World& physicsWorld);
        void createGroundBox(b2World& world, float x, float y, float density, float friction, std::string type);
        void createBox(b2World& world, int x, int y, float density, float friction, int team, std::string type);
        void createGib(b2World& world, int x, int y, sf::Color color, bool isFloating);
        void createGibBunch(b2World& world, int amount, int x, int y, sf::Color color, bool isFloating);

        void deleteBoxById(int id);
        void deleteGibById(int id);
        void deleteGroundBoxById(std::string id);

        void toggleBoxSelect(int id);
        void deselectAllBoxes();
        void stopAllActions();
        void changeBoxTeam(int id, int team);

        void setBoxTargetArea(int id, TargetArea target);
        void setBunchTargetArea(std::vector<int> ids, TargetArea target);

        void update(Events gameEvents, b2World& world, Player& player);
        void draw(sf::RenderWindow& window, b2Vec2 winTopLeft, b2Vec2 winBottomRight, bool isMinimap);

        int getAllyBoxAmount();
        int getAllBoxChargeAmount();
        int getChargedBox();

        bool groundBoxExists(std::string id);

        Timer completionTimer;
        bool completionStarted;
    protected:
    private:
        std::map<std::string, Box*> groundBoxes;
        std::vector<Box*> boxes;
        std::vector<Gibs*> gibs;

        void createWorldBoundaries(b2World& world);
        std::string getRandomBoxType(bool isEnemy);

        Timer resourceTimer;

        short int getRandomTeam(std::string type);
};

#endif // WORLD_H
