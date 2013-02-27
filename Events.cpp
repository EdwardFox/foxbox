#include "Events.h"

Events::Events()
{
    resetHeldStates();
}

Events::~Events()
{

}

void Events::processEvents(sf::RenderWindow& window, sf::Event& event)
{
    // Calculate coordinate adjustment factors
    int viewShiftX = window.getSize().x/2 - window.getView().getCenter().x;
    int viewShiftY = window.getSize().y/2 - window.getView().getCenter().y;

    resetStates();

    while(window.pollEvent(event))
    {
        // Close Event
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Mouse
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                lastMouseLeft.x = sf::Mouse::getPosition(window).x - viewShiftX;
                lastMouseLeft.y = sf::Mouse::getPosition(window).y - viewShiftY;
                mouseLeftClicked = true;
                mouseLeftHeld = true;
            }

            if(event.mouseButton.button == sf::Mouse::Right)
            {
                lastMouseRight.x = sf::Mouse::getPosition(window).x - viewShiftX;
                lastMouseRight.y = sf::Mouse::getPosition(window).y - viewShiftY;
                mouseRightClicked = true;
                mouseRightHeld = true;
            }
        }

        if(event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mouseLeftHeld = false;
            }

            if(event.mouseButton.button == sf::Mouse::Right)
            {
                mouseRightHeld = false;
            }
        }

        // Get last mouse coordinates
        lastMouse.x = sf::Mouse::getPosition(window).x - viewShiftX;
        lastMouse.y = sf::Mouse::getPosition(window).y - viewShiftY;

        // Get last mouse coordinates relative to window
        lastMouseWindow.x = sf::Mouse::getPosition(window).x;
        lastMouseWindow.y = sf::Mouse::getPosition(window).y;

        // Keyboard
        if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::A :
                    isA = true;
                    isAHeld = true;
                    break;
                case sf::Keyboard::D :
                    isD = true;
                    isDHeld = true;
                    break;
                case sf::Keyboard::E :
                    isE = true;
                    isEHeld = true;
                    break;
                case sf::Keyboard::F :
                    isF = true;
                    isFHeld = true;
                    break;
                case sf::Keyboard::G :
                    isG = true;
                    isGHeld = true;
                    break;
                case sf::Keyboard::H :
                    isH = true;
                    isHHeld = true;
                    break;
                case sf::Keyboard::I :
                    isI = true;
                    isIHeld = true;
                    break;
                case sf::Keyboard::J :
                    isJ = true;
                    isJHeld = true;
                    break;
                case sf::Keyboard::K :
                    isK = true;
                    isKHeld = true;
                    break;
                case sf::Keyboard::L :
                    isL = true;
                    isLHeld = true;
                    break;
                case sf::Keyboard::O :
                    isO = true;
                    isOHeld = true;
                    break;
                case sf::Keyboard::P :
                    isP = true;
                    isPHeld = true;
                    break;
                case sf::Keyboard::Q :
                    isQ = true;
                    isQHeld = true;
                    break;
                case sf::Keyboard::R :
                    isR = true;
                    isRHeld = true;
                    break;
                case sf::Keyboard::S :
                    isS = true;
                    isSHeld = true;
                    break;
                case sf::Keyboard::T :
                    isT = true;
                    isTHeld = true;
                    break;
                case sf::Keyboard::U :
                    isU = true;
                    isUHeld = true;
                    break;
                case sf::Keyboard::W :
                    isW = true;
                    isWHeld = true;
                    break;
                case sf::Keyboard::Y :
                    isY = true;
                    isYHeld = true;
                    break;
                case sf::Keyboard::Z :
                    isZ = true;
                    isZHeld = true;
                    break;
                case sf::Keyboard::Space :
                    isSpace = true;
                    isSpaceHeld = true;
                    break;
                case sf::Keyboard::LControl :
                    isLCTRL = true;
                    isLCTRLHeld = true;
                    break;
                case sf::Keyboard::LAlt :
                    isALT = true;
                    isALTHeld = true;
                    break;
                case sf::Keyboard::Num1 :
                    isNum1 = true;
                    isNum1Held = true;
                    break;
                case sf::Keyboard::Num2 :
                    isNum2 = true;
                    isNum2Held = true;
                    break;
                case sf::Keyboard::Num3 :
                    isNum3 = true;
                    isNum3Held = true;
                    break;
                case sf::Keyboard::Num4 :
                    isNum4 = true;
                    isNum4Held = true;
                    break;
                case sf::Keyboard::Num5 :
                    isNum5 = true;
                    isNum5Held = true;
                    break;
                case sf::Keyboard::Numpad1 :
                    isNumPad1 = true;
                    isNumPad1Held = true;
                    break;
                case sf::Keyboard::Numpad2 :
                    isNumPad2 = true;
                    isNumPad2Held = true;
                    break;
                case sf::Keyboard::Numpad3 :
                    isNumPad3 = true;
                    isNumPad3Held = true;
                    break;
                case sf::Keyboard::Numpad4 :
                    isNumPad4 = true;
                    isNumPad4Held = true;
                    break;
                case sf::Keyboard::Escape :
                    isEsc = true;
                    break;
            }
        }

        if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::A :
                    isAHeld = false;
                    break;
                case sf::Keyboard::D :
                    isDHeld = false;
                    break;
                case sf::Keyboard::E :
                    isEHeld = false;
                    break;
                case sf::Keyboard::F :
                    isFHeld = false;
                    break;
                case sf::Keyboard::G :
                    isGHeld = false;
                    break;
                case sf::Keyboard::H :
                    isHHeld = false;
                    break;
                case sf::Keyboard::I :
                    isIHeld = false;
                    break;
                case sf::Keyboard::J :
                    isJHeld = false;
                    break;
                case sf::Keyboard::K :
                    isKHeld = false;
                    break;
                case sf::Keyboard::L :
                    isLHeld = false;
                    break;
                case sf::Keyboard::O :
                    isOHeld = false;
                    break;
                case sf::Keyboard::P :
                    isPHeld = false;
                    break;
                case sf::Keyboard::Q :
                    isQHeld = false;
                    break;
                case sf::Keyboard::R :
                    isRHeld = false;
                    break;
                case sf::Keyboard::S :
                    isSHeld = false;
                    break;
                case sf::Keyboard::T :
                    isTHeld = false;
                    break;
                case sf::Keyboard::U :
                    isUHeld = false;
                    break;
                case sf::Keyboard::W :
                    isWHeld = false;
                    break;
                case sf::Keyboard::Y :
                    isYHeld = false;
                    break;
                case sf::Keyboard::Z :
                    isZHeld = false;
                    break;
                case sf::Keyboard::Space :
                    isSpaceHeld = false;
                    break;
                case sf::Keyboard::LControl :
                    isLCTRLHeld = false;
                    break;
                case sf::Keyboard::LAlt :
                    isALTHeld = false;
                    break;
                case sf::Keyboard::Num1 :
                    isNum1Held = false;
                    break;
                case sf::Keyboard::Num2 :
                    isNum2Held = false;
                    break;
                case sf::Keyboard::Num3 :
                    isNum3Held = false;
                    break;
                case sf::Keyboard::Num4 :
                    isNum4Held = false;
                    break;
                case sf::Keyboard::Num5 :
                    isNum5Held = false;
                    break;
                case sf::Keyboard::Numpad1 :
                    isNumPad1Held = false;
                    break;
                case sf::Keyboard::Numpad2 :
                    isNumPad2Held = false;
                    break;
                case sf::Keyboard::Numpad3 :
                    isNumPad3Held = false;
                    break;
                case sf::Keyboard::Numpad4 :
                    isNumPad4Held = false;
                    break;
            }
        }

        if(event.type == sf::Event::LostFocus || event.type == sf::Event::MouseLeft)
        {
            resetHeldStates();
        }
    }
}

void Events::resetStates()
{
    // Reset Mouse States
    mouseLeftClicked = false;
    mouseRightClicked = false;

    // Reset Keyboard States
    isA = false;
    isD = false;
    isE = false;
    isF = false;
    isG = false;
    isH = false;
    isI = false;
    isJ = false;
    isK = false;
    isL = false;
    isO = false;
    isP = false;
    isQ = false;
    isR = false;
    isS = false;
    isT = false;
    isU = false;
    isW = false;
    isY = false;
    isZ = false;
    isSpace = false;
    isLCTRL = false;
    isALT = false;
    isNum1 = false;
    isNum2 = false;
    isNum3 = false;
    isNum4 = false;
    isNum5 = false;
    isNumPad1 = false;
    isNumPad2 = false;
    isNumPad3 = false;
    isNumPad4 = false;
    isEsc = false;
}

void Events::resetHeldStates()
{
    mouseLeftHeld = false;
    mouseLeftLayer = LAYER_NONE;
    mouseRightHeld = false;
    mouseRightLayer = LAYER_NONE;

    isAHeld = false;
    isDHeld = false;
    isEHeld = false;
    isFHeld = false;
    isGHeld = false;
    isHHeld = false;
    isIHeld = false;
    isJHeld = false;
    isKHeld = false;
    isLHeld = false;
    isOHeld = false;
    isPHeld = false;
    isQHeld = false;
    isRHeld = false;
    isSHeld = false;
    isTHeld = false;
    isUHeld = false;
    isWHeld = false;
    isYHeld = false;
    isZHeld = false;
    isSpaceHeld = false;
    isLCTRLHeld = false;
    isALTHeld = false;
    isNum1Held = false;
    isNum2Held = false;
    isNum3Held = false;
    isNum4Held = false;
    isNum5Held = false;
    isNumPad1Held = false;
    isNumPad2Held = false;
    isNumPad3Held = false;
    isNumPad4Held = false;
}
