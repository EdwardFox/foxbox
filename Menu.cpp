#include "Menu.h"

Menu::Menu()
{
    //ctor
}

Menu::~Menu()
{
    //dtor
}

void Menu::update(Events& gameEvents)
{
    if(gameEvents.isEsc)
    {
        if(global_isPaused)
        {
            global_isPaused = false;
            global_isMenu = false;
        }
    }

    for(unsigned int i = 0; i < elements.size(); i++)
    {
        elements.at(i)->update(gameEvents);
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    // Background
    sf::Vertex rectangle[] =
    {
        sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Color(155, 155, 155, 255)),
        sf::Vertex(sf::Vector2f(window_width, 0.0f), sf::Color(155, 155,155, 255)),
        sf::Vertex(sf::Vector2f(window_width, window_height), sf::Color(255, 255, 255, 255)),
        sf::Vertex(sf::Vector2f(0.0f, window_height), sf::Color(255, 255, 255, 255))
    };
    window.draw(rectangle, 4, sf::Quads);

    for(unsigned int i = 0; i < elements.size(); i++)
    {
        elements.at(i)->draw(window);
    }
}

MenuImage* Menu::addImage(int x, int y, int width, int height, sf::Texture& texture)
{
    MenuImage* temp = new MenuImage(x, y, width, height, texture);
    elements.push_back(temp);

    return temp;
}

MenuButton* Menu::addButton(int x, int y, int width, int height, sf::Texture& texture)
{
    MenuButton* temp = new MenuButton(x, y, width, height, texture);
    elements.push_back(temp);

    return temp;
}
