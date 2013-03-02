#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "MenuImage.h"


class MenuButton : public MenuImage
{
    public:
        MenuButton();
        MenuButton(int x, int y, int width, int height, sf::Texture& texture);
        virtual ~MenuButton();

        void update(Events& gameEvents);
        void draw(sf::RenderWindow& window);

        void enableBackground(bool enabled, sf::Color color, sf::Color backgroundBorder);

        void (*callbackFunc)();
    protected:
        bool isMouseOnButton(Events& gameEvents);

        bool isBackgroundEnabled;
        sf::Color backgroundColor;
        sf::Color backgroundBorderColor;

        float clickTime;
        float clickDelay;
        bool isClicked;

    private:
};

#endif // MENUBUTTON_H
