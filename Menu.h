#ifndef MENU_H
#define MENU_H

#include "Events.h"
#include "utility.h"
#include "MenuElement.h"
#include "MenuImage.h"
#include "MenuButton.h"

class Menu
{
    public:
        Menu();
        virtual ~Menu();

        void update(Events& gameEvents);
        void draw(sf::RenderWindow& window);

        MenuImage* addImage(int x, int y, int width, int height, sf::Texture& texture);
        MenuButton* addButton(int x, int y, int width, int height, sf::Texture& texture);

    protected:
        std::vector<MenuElement*> elements;

    private:
};

#endif // MENU_H
