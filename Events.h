#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "utility.h"

class Events
{
    public:
        Events();
        virtual ~Events();

        void processEvents(sf::RenderWindow& window, sf::Event& event);
        // Mouse
        b2Vec2 lastMouse;
        b2Vec2 lastMouseWindow;

        b2Vec2 lastMouseLeft;
        bool mouseLeftClicked;
        bool mouseLeftHeld;
        short unsigned int mouseLeftLayer;

        b2Vec2 lastMouseRight;
        bool mouseRightClicked;
        bool mouseRightHeld;
        short unsigned int mouseRightLayer;

        // Keyboard
        bool isA;
        bool isAHeld;

        bool isD;
        bool isDHeld;

        bool isE;
        bool isEHeld;

        bool isF;
        bool isFHeld;

        bool isG;
        bool isGHeld;

        bool isH;
        bool isHHeld;

        bool isI;
        bool isIHeld;

        bool isJ;
        bool isJHeld;

        bool isK;
        bool isKHeld;

        bool isL;
        bool isLHeld;

        bool isO;
        bool isOHeld;

        bool isP;
        bool isPHeld;

        bool isQ;
        bool isQHeld;

        bool isR;
        bool isRHeld;

        bool isS;
        bool isSHeld;

        bool isT;
        bool isTHeld;

        bool isU;
        bool isUHeld;

        bool isW;
        bool isWHeld;

        bool isY;
        bool isYHeld;

        bool isZ;
        bool isZHeld;

        bool isSpace;
        bool isSpaceHeld;

        bool isLCTRL;
        bool isLCTRLHeld;

        bool isALT;
        bool isALTHeld;

        bool isNum1;
        bool isNum1Held;

        bool isNum2;
        bool isNum2Held;

        bool isNum3;
        bool isNum3Held;

        bool isNum4;
        bool isNum4Held;

        bool isNum5;
        bool isNum5Held;

        bool isNumPad1;
        bool isNumPad1Held;

        bool isNumPad2;
        bool isNumPad2Held;

        bool isNumPad3;
        bool isNumPad3Held;

        bool isNumPad4;
        bool isNumPad4Held;
    protected:
    private:
        void resetStates();
        void resetHeldStates();
};

#endif // EVENTS_H_INCLUDED
