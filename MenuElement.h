#ifndef MENUELEMENT_H
#define MENUELEMENT_H

#include "Events.h"
#include "utility.h"

class MenuElement
{
    public:
        MenuElement();
        virtual ~MenuElement();

        virtual void update(Events& gameEvents);
        virtual void draw(sf::RenderWindow& window);

        sf::Vector2i getPosition();
        void setPosition(int x, int y);

        bool isDrawn;
    protected:
        int x;
        int y;
    private:
};

#endif // MENUELEMENT_H
