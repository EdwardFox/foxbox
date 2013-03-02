#include "Player.h"

Player::Player()
{
    selectionAreaActive = false;
    selectionArea.setFillColor(sf::Color(0, 255, 55, 100));

    hasNewFocus = false;
    pushActive = false;
    transformBoxActive = false;
    pushBox = NULL;
    transformBox = NULL;
    forcePushPower = 100.0f;
    resourceAmount = 0;
    transformCost = 0;
    resourceConsumption = sf::Text("", font_default, DEFAULT_TEXT_SIZE);
    resourceConsumption.setCharacterSize(15);
    resourceConsumption.setStyle(sf::Text::Bold);
}

Player::~Player()
{
    //dtor
}

void Player::update(b2World &physicsWorld, Events gameEvents, World &gameWorld)
{
    // ------------------------------- Keyboard -------------------------------
    if(gameEvents.isNum1)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.getAllAlliesByType("box");
    }

    if(gameEvents.isNum2)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.getAllAlliesByType("magicbox");
    }

    if(gameEvents.isNum3)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.getAllAlliesByType("chargebox");
    }

    if(gameEvents.isNum4)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.getAllAlliesByType("floatbox");
    }

    if(gameEvents.isNum5)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.getAllAlliesByType("boombox");
    }

    if(gameEvents.isE)
    {
        selectedBoxes.clear();
        selectedBoxes = gameWorld.selectAllDynamicBoxes();
    }

//    if(gameEvents.isF)
//    {
//        gameWorld.stopAllActions();
//    }

//    if(gameEvents.isG || gameEvents.isGHeld)
//    {
//        gameWorld.createGib(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, sf::Color(255, 0, 0, 255), false);
//        gameWorld.createGib(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, sf::Color(0, 255, 0, 255), false);
//        gameWorld.createGib(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, sf::Color(0, 0, 255, 255), false);
//    }

//    if(gameEvents.isH)
//    {
//        gameWorld.createBox(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, 30.0f, 0.35f, 1, "chargebox");
//    }

//    if(gameEvents.isI)
//    {
//        if(debug_global_detonateOnContact)
//        {
//            std::cout << "GLOBAL: BoomBox detonate on contact disabled." << std::endl;
//            debug_global_detonateOnContact = false;
//        }
//        else
//        {
//            std::cout << "GLOBAL: BoomBox detonate on contact enabled." << std::endl;
//            debug_global_detonateOnContact = true;
//        }
//    }

//    if(gameEvents.isJ)
//    {
//        gameWorld.createBox(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, 30.0f, 0.35f, 1, "box");
//    }

//    if(gameEvents.isK)
//    {
//        gameWorld.createBox(physicsWorld, gameEvents.lastMouse.x, gameEvents.lastMouse.y, 30.0f, 0.35f, 1, "boombox");
//    }

//    if(gameEvents.isL)
//    {
//        int id = gameWorld.getBoxIdByCoordinates(gameEvents.lastMouse.x, gameEvents.lastMouse.y);
//        b2Vec2 pos = getGroundCoordsByCoordinates(gameEvents.lastMouse.x, gameEvents.lastMouse.y);
//        std::string groundId = getIndexByGroundCoordinates(pos.x, pos.y);
//
//        // Check if we have selected a box
//        if(id >= 0)
//        {
//            // Check if element exists in selection and delete it
//            for(unsigned int i = 0; i < selectedBoxes.size(); i++)
//            {
//                if(selectedBoxes.at(i) == id)
//                {
//                    selectedBoxes.erase(selectedBoxes.begin() + i);
//                }
//            }
//
//            // Delete box
//            //gameWorld.deleteBoxById(id);
//            gameWorld.getBoxById(id)->isDead = true;
//
//            // Rebuild selected boxes vector
//            selectedBoxes.clear();
//            selectedBoxes = gameWorld.getSelectedBoxes();
//        }
//
//        try
//        {
//            gameWorld.getGroundBoxes()->at(groundId)->isDead = true;
//        }
//        catch(std::out_of_range ex)
//        {
//            //std::cout << "No groundBox found." << std::endl;
//        }
//    }
//
//    if(gameEvents.isO)
//    {
//        gameWorld.regenerateWorld(physicsWorld);
//    }

    if(gameEvents.isP)
    {
        if(global_isPaused)
        {
            global_isPaused = false;
        }
        else
        {
            global_isPaused = true;
        }
    }

    if(gameEvents.isQ)
    {
        if(global_showFriendlyBoxInfo)
        {
            global_showFriendlyBoxInfo = false;
        }
        else
        {
            global_showFriendlyBoxInfo = true;
        }
    }

    if(gameEvents.isR)
    {
        if(selectedBoxes.size() > 0)
        {
            for(unsigned int i = 0; i < selectedBoxes.size(); i++)
            {
                if(!gameWorld.getBoxById(selectedBoxes.at(i))->repeatAttack)
                {
                    gameWorld.getBoxById(selectedBoxes.at(i))->isAttacking = true;
                    gameWorld.getBoxById(selectedBoxes.at(i))->repeatAttack = true;
                }
                else
                {
                    gameWorld.getBoxById(selectedBoxes.at(i))->resetAttack();
                }

            }
        }
    }

    if(gameEvents.isT)
    {
        selectedBoxes.clear();

        int selectedBox = gameWorld.getChargedBox();

        if(selectedBox >= 0)
        {
            selectedBoxes.push_back(selectedBox);

            hasNewFocus = true;

            Box* b = gameWorld.getBoxById(selectedBox);
            newFocus.x = b->getBody()->GetPosition().x * SCALE;
            newFocus.y = b->getBody()->GetPosition().y * SCALE;
        }
    }

    if(gameEvents.isU)
    {
        if(music.getStatus() == sf::Music::Playing)
        {
            music.pause();
        }
        else
        {
            music.play();
        }
    }

    if(gameEvents.isY)
    {
        if(global_enableAllyAI)
        {
            global_enableAllyAI = false;
        }
        else
        {
            global_enableAllyAI = true;
        }
    }

    if(gameEvents.isZ)
    {
        music.stop();
    }

    if(gameEvents.isSpace)
    {
        if(selectedBoxes.size() > 0)
        {
            for(unsigned int i = 0; i < selectedBoxes.size(); i++)
            {
                gameWorld.getBoxById(selectedBoxes.at(i))->isAttacking = true;
                gameWorld.getBoxById(selectedBoxes.at(i))->repeatAttack = false;
            }
        }
    }

//    if(gameEvents.isNumPad1)
//    {
//        if(debug_showTargetLines)
//        {
//            std::cout << "DEBUG: Target Lines disabled." << std::endl;
//            debug_showTargetLines = false;
//        }
//        else
//        {
//            std::cout << "DEBUG: Target Lines enabled." << std::endl;
//            debug_showTargetLines = true;
//        }
//    }
//
//    if(gameEvents.isNumPad2)
//    {
//        if(debug_showTargetArea)
//        {
//            std::cout << "DEBUG: Target Area disabled." << std::endl;
//            debug_showTargetArea = false;
//        }
//        else
//        {
//            std::cout << "DEBUG: Target Area enabled." << std::endl;
//            debug_showTargetArea = true;
//        }
//    }
//
//    if(gameEvents.isNumPad3)
//    {
//        if(debug_showStaticHealth)
//        {
//            std::cout << "DEBUG: Static Box Health hidden." << std::endl;
//            debug_showStaticHealth = false;
//        }
//        else
//        {
//            std::cout << "DEBUG: Static Box Health shown." << std::endl;
//            std::cout << "WARNING: Crash upon exit once health was shown. Known SFML bug." << std::endl;
//            debug_showStaticHealth = true;
//        }
//    }
//
//    if(gameEvents.isNumPad4)
//    {
//        if(debug_showInactiveGroundBoxes)
//        {
//            std::cout << "DEBUG: Inactive Ground Boxes hidden." << std::endl;
//            debug_showInactiveGroundBoxes = false;
//        }
//        else
//        {
//            std::cout << "DEBUG: Inactive Ground Boxes shown." << std::endl;
//            debug_showInactiveGroundBoxes = true;
//        }
//    }

    if(gameEvents.isEsc)
    {
        global_isMenu = true;
        global_isPaused = true;
    }

    // ------------------------------- Mouse -------------------------------
    if(gameEvents.mouseLeftClicked && gameEvents.isLCTRLHeld)
    {
        int id = gameWorld.getBoxIdByCoordinates(gameEvents.lastMouseLeft.x, gameEvents.lastMouseLeft.y);

        if(id >= 0 && gameWorld.getBoxById(id)->team == TEAM_ALLY)
        {
            if(std::find(selectedBoxes.begin(), selectedBoxes.end(), id) != selectedBoxes.end())
            {
                selectedBoxes.erase(std::remove(selectedBoxes.begin(), selectedBoxes.end(), id), selectedBoxes.end());
            }
            else
            {
                selectedBoxes.push_back(id);
            }

            gameWorld.toggleBoxSelect(id);
        }
    }
    else if(gameEvents.mouseLeftClicked)
    {
        int id = gameWorld.getBoxIdByCoordinates(gameEvents.lastMouseLeft.x, gameEvents.lastMouseLeft.y);

        if(id >= 0 && gameWorld.getBoxById(id)->team == TEAM_ALLY)
        {
            clearActiveBoxes(gameWorld);
            selectedBoxes.push_back(id);
            gameWorld.toggleBoxSelect(id);
        }
        else
        {
            clearActiveBoxes(gameWorld);
        }
    }

    if(gameEvents.mouseLeftHeld)
    {
        setSelectionArea(gameEvents);
    }
    else
    {
        if(selectionAreaActive)
        {
            // Do the actual selection
            std::vector<int> selection = gameWorld.getBoxesBySelection(sf::Vector2f(selectionAreaPosition.x, selectionAreaPosition.y), selectionAreaSize);
            selectedBoxes.insert(selectedBoxes.end(), selection.begin(), selection.end());
            selectionAreaActive = false;
        }
    }

    if(gameEvents.mouseRightClicked)
    {
        if(selectedBoxes.size() > 0)
        {
            float targetAreaSize = BOX_SIZE/4.0f + selectedBoxes.size() * 2.0f;

            gameWorld.setBunchTargetArea(selectedBoxes, TargetArea(gameEvents.lastMouseRight.x, gameEvents.lastMouseRight.y, targetAreaSize));

            mTargetMarker.createCircle(gameEvents.lastMouseRight.x, gameEvents.lastMouseRight.y, 0.2f, 50.0f, sf::Color(250, 250, 250, 255));
        }
    }

    if(gameEvents.mouseRightHeld)
    {
        if(selectedBoxes.size() == 0)
        {
            // Dynamics
            if(pushBox == NULL)
            {
                int id = gameWorld.getBoxIdByCoordinates(gameEvents.lastMouseRight.x, gameEvents.lastMouseRight.y);

                if(id >= 0)
                {
                    pushBox = gameWorld.getBoxById(id);

                    if(pushBox->type == "floatbox")
                    {
                        pushBox = NULL;
                    }
                    else
                    {
                        setPushVisuals(gameEvents);
                    }
                }
            }
            else
            {
                setPushVisuals(gameEvents);
            }

            // Statics
            if(transformBox == NULL)
            {
                b2Vec2 pos = getGroundCoordsByCoordinates(gameEvents.lastMouseRight.x, gameEvents.lastMouseRight.y);
                std::string index = getIndexByGroundCoordinates(pos.x, pos.y);

                try
                {
                    transformBox = gameWorld.getGroundBoxes()->at(index);

                    if(!transformBox->getBody()->IsActive() || transformBox->team == TEAM_ENEMY)
                    {
                        transformBox = NULL;
                    }
                }
                catch(std::out_of_range)
                {
                    // blub
                }
            }
            else
            {
                setBoxTransformVisuals(gameEvents);
            }
        }
    }
    else
    {
        if(pushBox != NULL)
        {
            forcePushBox(gameEvents.lastMouse.x, gameEvents.lastMouse.y);
            pushActive = false;
            pushBox = NULL;
        }

        if(transformBox != NULL)
        {
            transformBoxToAlly();
            transformBoxActive = false;
            transformBox = NULL;
        }

        transformCost = 0;
    }

    mTargetMarker.update();
}

void Player::clearActiveBoxes(World& gameWorld)
{
    for(unsigned int i = 0; i < selectedBoxes.size(); i++)
    {
        gameWorld.toggleBoxSelect(selectedBoxes.at(i));
    }

    selectedBoxes.clear();
}

void Player::draw(sf::RenderWindow& window)
{
    if(selectionAreaActive)
    {
        window.draw(selectionArea);
    }

    if(pushActive)
    {
        window.draw(line, 2, sf::Lines);
        window.draw(resourceConsumption);
    }

    if(transformBoxActive)
    {
        window.draw(resourceConsumption);
    }

    mTargetMarker.draw(window);
}

void Player::setBoxTransformVisuals(Events gameEvents)
{
    if(transformBox->type == "box")
    {
        if(transformCost < BOX_RELEASE_COST * BOX_VALUE)
        {
            transformCost += 2;
        }
    }
    else if(transformBox->type == "chargebox")
    {
        if(transformCost < BOX_RELEASE_COST * CHARGEBOX_VALUE)
        {
            transformCost += 2;
        }
    }
    else if(transformBox->type == "magicbox")
    {
        if(transformCost < BOX_RELEASE_COST * MAGICBOX_VALUE)
        {
            transformCost += 2;
        }
    }
    else if(transformBox->type == "floatbox")
    {
        if(transformCost < BOX_RELEASE_COST * FLOATBOX_VALUE)
        {
            transformCost += 2;
        }
    }
    else if(transformBox->type == "boombox")
    {
        if(transformCost < BOX_RELEASE_COST * BOOMBOX_VALUE)
        {
            transformCost += 2;
        }
    }

    if(transformCost <= resourceAmount)
    {
        resourceConsumption.setColor(sf::Color::Green);
    }
    else
    {
        resourceConsumption.setColor(sf::Color::Red);
    }

    std::stringstream consump;
    consump << "-" << transformCost << " Res";

    resourceConsumption.setString(consump.str());
    resourceConsumption.setPosition(gameEvents.lastMouse.x + 10, gameEvents.lastMouse.y);

    transformBoxActive = true;
}

void Player::setPushVisuals(Events gameEvents)
{
    // Line
    sf::Vector2f a(pushBox->getBody()->GetPosition().x * SCALE, pushBox->getBody()->GetPosition().y * SCALE);
    sf::Vector2f b(gameEvents.lastMouse.x, gameEvents.lastMouse.y);
    line[0] = a;
    line[1] = b;

    // Consumption
    resourceConsumption.setPosition(gameEvents.lastMouse.x + 10, gameEvents.lastMouse.y);

    float boxX = pushBox->getBody()->GetPosition().x * SCALE;
    float boxY = pushBox->getBody()->GetPosition().y * SCALE;
    float distX = gameEvents.lastMouse.x - boxX;
    float distY = gameEvents.lastMouse.y - boxY;
    float dist = sqrt(distX*distX + distY*distY);
    int amount = dist/10.0f;

    if(pushBox->team == TEAM_NEUTRAL)
    {
        amount *= 1.5f;
    }
    else if(pushBox->team == TEAM_ENEMY)
    {
        amount *= 2.0f;
    }

    std::stringstream consump;
    consump << "-" << amount << " Res";

    resourceConsumption.setString(consump.str());

    if(amount <= resourceAmount)
    {
        line[0].color = sf::Color::Green;
        line[1].color = sf::Color::Green;
        resourceConsumption.setColor(sf::Color::Green);
    }
    else
    {
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        resourceConsumption.setColor((sf::Color::Red));
    }

    pushActive = true;
}

void Player::setSelectionArea(Events gameEvents)
{
    calcSelectionArea(gameEvents);

    // Visuals
    selectionArea.setPosition(selectionAreaPosition.x, selectionAreaPosition.y);
    selectionArea.setSize(selectionAreaSize);
    selectionAreaActive = true;
}

void Player::calcSelectionArea(Events gameEvents)
{
    selectionAreaPosition.x = gameEvents.lastMouseLeft.x;
    if(gameEvents.lastMouseLeft.x > gameEvents.lastMouse.x)
    {
        selectionAreaPosition.x = gameEvents.lastMouse.x;
    }

    selectionAreaPosition.y = gameEvents.lastMouseLeft.y;
    if(gameEvents.lastMouseLeft.y > gameEvents.lastMouse.y)
    {
        selectionAreaPosition.y = gameEvents.lastMouse.y;
    }

    selectionAreaSize.x = abs(gameEvents.lastMouseLeft.x - gameEvents.lastMouse.x);
    selectionAreaSize.y = abs(gameEvents.lastMouseLeft.y - gameEvents.lastMouse.y);
}

b2Vec2 Player::getNewFocus()
{
    hasNewFocus = false;
    return newFocus;
}

int Player::getResources()
{
    return resourceAmount;
}

void Player::addResources(int amount)
{
    resourceAmount += amount;
}

void Player::forcePushBox(int mouseX, int mouseY)
{
    float boxX = pushBox->getBody()->GetPosition().x * SCALE;
    float boxY = pushBox->getBody()->GetPosition().y * SCALE;
    float distX = mouseX - boxX;
    float distY = mouseY - boxY;
    float dist = sqrt(distX*distX + distY*distY);
    int amount = dist / 10.0f;

    if(pushBox->team == TEAM_NEUTRAL)
    {
        amount *= 1.5f;
    }
    else if(pushBox->team == TEAM_ENEMY)
    {
        amount *= 2.0f;
    }

    if(amount <= resourceAmount)
    {
        pushBox->getBody()->ApplyForceToCenter(b2Vec2(distX * forcePushPower, distY * forcePushPower));

        if(distX < 0)
        {
            pushBox->getBody()->SetAngularVelocity(-forcePushPower/10);
        }
        else
        {
            pushBox->getBody()->SetAngularVelocity(forcePushPower/10);
        }

        resourceAmount -= amount;
    }
}

void Player::transformBoxToAlly()
{
    bool transformCostSupplied = false;

    if(transformBox->type == "box")
    {
        if(transformCost == BOX_RELEASE_COST * BOX_VALUE)
        {
            transformCostSupplied = true;
        }
    }
    else if(transformBox->type == "chargebox")
    {
        if(transformCost == BOX_RELEASE_COST * CHARGEBOX_VALUE)
        {
            transformCostSupplied = true;
        }
    }
    else if(transformBox->type == "magicbox")
    {
        if(transformCost == BOX_RELEASE_COST * MAGICBOX_VALUE)
        {
            transformCostSupplied = true;
        }
    }
    else if(transformBox->type == "floatbox")
    {
        if(transformCost == BOX_RELEASE_COST * FLOATBOX_VALUE)
        {
            transformCostSupplied = true;
        }
    }
    else if(transformBox->type == "boomebox")
    {
        if(transformCost == BOX_RELEASE_COST * BOOMBOX_VALUE)
        {
            transformCostSupplied = true;
        }
    }

    if(transformCost <= resourceAmount && transformCostSupplied)
    {
        transformBox->changeTeam(TEAM_ALLY);
        transformBox->isFlaggedForConversion = true;

        resourceAmount -= transformCost;
    }
}
