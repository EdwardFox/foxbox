#ifndef MENUIMAGE_H
#define MENUIMAGE_H

#include "MenuElement.h"

class MenuImage : public MenuElement
{
    public:
        MenuImage();
        MenuImage(int x, int y, int width, int height, sf::Texture& texture);
        virtual ~MenuImage();

        void createImage(int x, int y, int width, int height, sf::Texture& texture);

        virtual void update(Events& gameEvents);
        virtual void draw(sf::RenderWindow& window);

        sf::Vector2i getSize();
        void setSize(int width, int height);
    protected:
        sf::Sprite sprite;
        int width;
        int height;

    private:
};

#endif // MENUIMAGE_H
