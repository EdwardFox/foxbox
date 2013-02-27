#include "MenuElement.h"

MenuElement::MenuElement()
{
    isDrawn = true;
}

MenuElement::~MenuElement()
{
    //dtor
}

void MenuElement::update(Events& gameEvents)
{

}

void MenuElement::draw(sf::RenderWindow& window)
{

}

sf::Vector2i MenuElement::getPosition()
{
    return sf::Vector2i(x, y);
}

void MenuElement::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}
