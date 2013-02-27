#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include "World.h"
#include "Timer.h"

class UI
{
    public:
        UI();
        virtual ~UI();

        void setSize(int width, int height);
        void update(World gameWorld, Player player);
        void draw(sf::RenderWindow& window);
        void init();
        void setGlobalMessage(std::string msg);

        int addText(std::string text, float x, float y, unsigned int size, sf::Color color);

        sf::View getView();
    protected:
    private:
        void setCenter();

        std::vector<sf::Text> texts;

        int id_boxAmount;
        int id_boxChargeAmount;
        int id_resources;
        int id_ai;
        int id_globalMessage;

        Timer globalMessageTimer;

        int width;
        int height;

        sf::View hudView;
};

#endif // UI_H
