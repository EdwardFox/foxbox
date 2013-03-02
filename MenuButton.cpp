#include "MenuButton.h"

MenuButton::MenuButton()
{
    isBackgroundEnabled = false;
    clickDelay = 0.1f * FRAMES_PER_SECOND;
    isClicked = false;
}

MenuButton::MenuButton(int x, int y, int width, int height, sf::Texture& texture) : MenuImage(x, y, width, height, texture)
{
    isBackgroundEnabled = false;
    clickDelay = 0.1f * FRAMES_PER_SECOND;
    isClicked = false;
}

MenuButton::~MenuButton()
{
    //dtor
}

void MenuButton::update(Events& gameEvents)
{
    MenuImage::update(gameEvents);

    if(isMouseOnButton(gameEvents))
    {
        sprite.setColor(sf::Color(100, 100, 100, 255));

        if(clickTime >= clickDelay && !isClicked && isDrawn)
        {
            menu_click.play();
            clickTime = 0.0f;
            isClicked = true;
        }


        if(gameEvents.mouseLeftClicked)
        {
            callbackFunc();
        }
    }
    else
    {
        sprite.setColor(sf::Color(255, 255, 255, 255));
        isClicked = false;
    }

    clickTime++;
}

void MenuButton::draw(sf::RenderWindow& window)
{
    if(isBackgroundEnabled && isDrawn)
    {
        sf::RectangleShape background;
        background.setPosition(x, y);
        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(backgroundColor);
        background.setOutlineColor(backgroundBorderColor);
        background.setOutlineThickness(1.0f);

        window.draw(background);
    }

    MenuImage::draw(window);

}

bool MenuButton::isMouseOnButton(Events& gameEvents)
{
    int mX = gameEvents.lastMouse.x;
    int mY = gameEvents.lastMouse.y;

    if(mX >= x && mX <= x + width && mY >= y && mY <= y + height)
    {
        return true;
    }

    return false;
}

void MenuButton::enableBackground(bool enabled, sf::Color color, sf::Color backgroundBorder)
{
    isBackgroundEnabled = enabled;
    backgroundColor = color;
    backgroundBorderColor = backgroundBorder;
}
