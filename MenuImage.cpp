#include "MenuImage.h"

MenuImage::MenuImage() : MenuElement()
{
    //ctor
}

MenuImage::MenuImage(int x, int y, int width, int height, sf::Texture& texture) : MenuElement()
{
    createImage(x, y, width, height, texture);
}

MenuImage::~MenuImage()
{
    //dtor
}

void MenuImage::update(Events& gameEvents)
{
    sprite.setPosition(x, y);
}

void MenuImage::draw(sf::RenderWindow& window)
{
    if(isDrawn)
    {
        window.draw(sprite);
    }
}

void MenuImage::createImage(int x, int y, int width, int height, sf::Texture& texture)
{
    setPosition(x, y);
    setSize(width, height);

    sprite.setPosition(x, y);
    sprite.setOrigin(0.0f, 0.0f);
    sprite.setTexture(texture);
}

sf::Vector2i MenuImage::getSize()
{
    return sf::Vector2i(width, height);
}

void MenuImage::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}
