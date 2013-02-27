#include "Box.h"

Box::Box(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes)
{
    this->groundBoxes = groundBoxes;
    this->boxes = boxes;

    init();
}

Box::~Box()
{
    //dtor
}

void Box::init()
{
    isActive = false;
    isDead = false;
    isAttacking = false;
    isNotified = false;
    isFlaggedForConversion = false;
    isConverted = false;
    isInfo = false;
    isSticky = false;
    team = TEAM_NEUTRAL;
    hasActiveTargetArea = false;
    target.position = sf::Vector2f(0.0f, 0.0f);
    target.radius = 0.0f;
    speed = 5.0f;
    stopSpeed = 1.5f;
    jumpHeight = 6.0f;
    isJumping = false;
    lastImpulseReceived = 0.0f;
    health = 100;
    type = "box";
    preJumpPosition = b2Vec2(0.0f, 0.0f);
    tagCharges = 1;
    stompJumpForce = 15000.0f;
    stompForce = 60000.0f;
    attackStarted = false;
    attackTimer.setThreshold(1.0f);
    attackTimer.endTimer();
    attackDamage = 30;
    damageResistance = 1.0f;
    hitTimer.setThreshold(0.05f);
    hitTimer.endTimer();
    hitNumberTimer.setThreshold(1.5f);
    hitNumberTimer.endTimer();
    aiTarget = NULL;
    attackRange = 300;
    aiAttackTimer.setThreshold(2.0f);
    aiAttackTimer.endTimer();
    contactSoundTimer.setThreshold(0.5f);
    contactSoundTimer.endTimer();

    stickyMaster = NULL;
    floatMaster = NULL;

    // Damage taken
    text_damageTaken = sf::Text("", font_default, DEFAULT_TEXT_SIZE);
    text_damageTaken.setColor(sf::Color::Red);
    text_damageTaken.setStyle(sf::Text::Bold);

    // Team, Type, Charges
    addBoxInfoLine();
    addBoxInfoLine();
    addBoxInfoLine();
    addBoxInfoLine();
}

void Box::updateTexture()
{
    if(body->IsActive())
    {
        setTexture(textures[type + "_ground"]);

        switch(team)
        {
            case TEAM_NEUTRAL:
                overlaySprite.setColor(sf::Color(255, 255, 255, 0));
                break;
            case TEAM_ALLY:
                setOverlayTexture(textures["overlay_allied"]);
                overlaySprite.setColor(sf::Color(255, 255, 255, 255));
                break;
            case TEAM_ENEMY:
                setOverlayTexture(textures["overlay_enemy"]);
                overlaySprite.setColor(sf::Color(255, 255, 255, 255));
                break;
        }
    }
    else
    {
        setTexture(textures["box_ground"]);
    }


    if(isActive)
    {
        setOverlayTexture(textures["overlay_selected"]);
        overlaySprite.setColor(sf::Color(255, 255, 255, 255));
    }
}

void Box::createBox(b2World& physicsWorld, b2Vec2 position, b2Vec2 size, float density, float friction, bool isDynamic, uint16 categoryBits, uint16 maskBits)
{
    // Physics body initialization
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(position.x/SCALE, position.y/SCALE);

    if(isDynamic)
    {
        bodyDef.type = b2_dynamicBody;
    }
    else
    {
        bodyDef.type = b2_staticBody;
    }

    body = physicsWorld.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((size.x/2)/SCALE, (size.y/2)/SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    //fixtureDef.restitution = 0.05f;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;

    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    if(body->GetType() ==  b2_staticBody)
    {
        setBodyActive(false);
    }

    // Sprite initilization
    sprite.setOrigin(size.x/2.0f, size.y/2.0f);
    sprite.setPosition(position.x, position.y);
    overlaySprite.setOrigin(size.x/2.0f, size.y/2.0f);
    overlaySprite.setPosition(position.x, position.y);
}

void Box::setTexture(sf::Texture &tex)
{
    sprite.setTexture(tex);
}

void Box::setOverlayTexture(sf::Texture &tex)
{
    overlaySprite.setTexture(tex);
}

bool Box::groundBoxExists(const std::string id)
{
    if(groundBoxes->find(id) == groundBoxes->end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Box::update(Events gameEvents)
{
    // ------------------------------- ACTIVE -------------------------------
    if(body->IsActive())
    {
        // Update UserData
        body->SetUserData(this);

        // ------------------------------- Dynamic only -------------------------------
        if(body->GetType() == b2_dynamicBody)
        {
            sprite.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
            sprite.setRotation(180/b2_pi * body->GetAngle());
            overlaySprite.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
            overlaySprite.setRotation(180/b2_pi * body->GetAngle());

            lastMouse.x = gameEvents.lastMouse.x;
            lastMouse.y = gameEvents.lastMouse.y;

            // Movement
            moveToTargetArea();

            // AI
            ai();

            // Atack
            attack();
        }
        else // ------------------------------- Static only -------------------------------
        {
            ai();
            attack();
            checkNotifications();
            checkForConversion();
        }
        // ------------------------------- Both -------------------------------
        processImpulse();

        // Deletes boxes that have fallen out of the world
        checkHeight();

        // MouseOver
        if(isSelected(gameEvents.lastMouse.x, gameEvents.lastMouse.y))
        {
            isInfo = true;
            sprite.setColor(sf::Color(120, 120, 120, 255));
        }
        else
        {
            isInfo = false;
            sprite.setColor(sf::Color(255, 255, 255, 255));
        }
    }
    else // ------------------------------- INACTIVE -------------------------------
    {
        if(body->GetType() == b2_staticBody)
        {
            checkNotifications();

            if(isSelected(gameEvents.lastMouse.x, gameEvents.lastMouse.y))
            {
                sprite.setColor(sf::Color(120, 120, 120, 255));
            }
            else
            {
                sprite.setColor(sf::Color(200, 200, 200, 255));
            }
        }
    }

    // ------------------------------- Independant of active status -------------------------------
    // Check for minimap
//    float percX = gameEvents.lastMouseWindow.x / window_width;
//    float percY = gameEvents.lastMouseWindow.y / window_height;
//
//    if(percX >= 0.0f && percX <= minimapSize && percY >= 1.0f - minimapSize && percY <= 1.0f)
//    {
//        sprite.setColor(sf::Color(255, 255, 255, 255));
//    }

    // Flicker as long as the timer is active
    if(!hitTimer.timeReached())
    {
        sprite.setColor(sf::Color::Red);
    }

    // Update timer
    hitTimer.update();
    hitNumberTimer.update();
}

void Box::draw(sf::RenderWindow& window)
{
    // Draw Box
    if(body->IsActive() || (!body->IsActive() && debug_showInactiveGroundBoxes))
    {
        window.draw(sprite);
        window.draw(overlaySprite);
    }

    // Dynamic
    if(body->GetType() == b2_dynamicBody)
    {
        // Draw target
        if(hasActiveTargetArea)
        {
            // Line
            if(debug_showTargetLines)
            {
                sf::Vector2f a(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
                sf::Vector2f b(target.position.x, target.position.y);
                sf::Vertex line[2] = {a, b};
                line[0].color = sf::Color::Red;
                line[1].color = sf::Color::Red;

                window.draw(line, 2, sf::Lines);
            }

            // Area
            if(debug_showTargetArea)
            {
                sf::RectangleShape rect;
                sf::Vector2f pos(target.originalPosition.x - target.radius, target.originalPosition.y - target.radius);
                sf::Vector2f size(abs((target.originalPosition.x - target.radius) - (target.originalPosition.x + target.radius)), abs((target.originalPosition.y - target.radius) - (target.originalPosition.y + target.radius)));

                rect.setPosition(pos);
                rect.setSize(size);
                rect.setFillColor(sf::Color(0, 100, 200, 25));

                window.draw(rect);
            }
        }
    }
    else // Static
    {
        // Health
        if(debug_showStaticHealth)
        {
            std::stringstream healthString;
            healthString << health;

            sf::Text textHealth;
            textHealth.setString(healthString.str());
            textHealth.setCharacterSize(10);
            textHealth.setPosition(body->GetPosition().x * SCALE - BOX_SIZE/2, body->GetPosition().y * SCALE - BOX_SIZE/2);
            textHealth.setColor(sf::Color::Black);

            window.draw(textHealth);
        }
    }
}

sf::Sprite* Box::getSprite()
{
    return &sprite;
}

b2Body* Box::getBody()
{
    return body;
}

bool Box::isSelected(int mouseX, int mouseY)
{
    int minX = sprite.getPosition().x - BOX_SIZE/2;
    int maxX = minX + BOX_SIZE;
    int minY = sprite.getPosition().y - BOX_SIZE/2;
    int maxY = minY + BOX_SIZE;

    if(mouseX <= maxX && mouseX >= minX && mouseY <= maxY && mouseY >= minY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Box::startContact(Box* box, bool isDynamic)
{
    if(isDynamic)
    {
        contactDynamicList.push_back(box);
    }
    else
    {
        contactStaticList.push_back(box);
    }
}

void Box::endContact(Box* box,bool isDynamic)
{
    if(isDynamic)
    {
        contactDynamicList.erase(std::remove(contactDynamicList.begin(), contactDynamicList.end(), box), contactDynamicList.end());
    }
    else
    {
        contactStaticList.erase(std::remove(contactStaticList.begin(), contactStaticList.end(), box), contactStaticList.end());
    }
}

void Box::moveToTargetArea()
{
    // Move only if we have a target
    if(hasActiveTargetArea)
    {
        repeatAttack = false;

        // Get center and calculate directions
        float centerX = body->GetPosition().x * SCALE;
        float centerY = body->GetPosition().y * SCALE;
        float cSpeed = speed;
        float cJumpHeight = jumpHeight;

        if(centerX > target.position.x)
        {
            cSpeed = -cSpeed;
        }

        if(centerY < target.position.y)
        {
            cJumpHeight = 0.0f;
        }

        // STOP CONDITION: We are within target boundaries
        if(centerX <= target.position.x + target.radius && centerX >= target.position.x - target.radius && centerY <= target.position.y + target.radius && centerY >= target.position.y - target.radius)
        {
            hasActiveTargetArea = false;
            //dampenedJump();
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        }
        else
        {
            // Can jump again if we touch a static box
            if(contactStaticList.size() > 0)
            {
                isJumping = false;
            }
            else
            {
                // Can jump if we are not in motion anylonger
                // Prevents getting stuck on dynamic boxes
                if(!isInMotion())
                {
                    isJumping = false;
                }
            }

            // If either of our dynamic contacts is a float box
            // we can jump again
            for(unsigned int i = 0; i < contactDynamicList.size(); i++)
            {
                if(contactDynamicList.at(i)->type == "floatbox" && ((team == TEAM_ALLY && contactDynamicList.at(i)->team != TEAM_ENEMY) || (team == TEAM_ENEMY && contactDynamicList.at(i)->team != TEAM_ALLY)))
                {
                    isJumping = false;
                }
            }

            // We can only jump if we are not currently jumping
            if(!isJumping)
            {
                // If we are at the same horizontal position from before the last jump, we are probably stuck.
                // In this case use normal jump height instead of 0 to enable climbing
                if((int)preJumpPosition.x == (int)(centerX))
                {
                    cJumpHeight = jumpHeight;
                }

                preJumpPosition = b2Vec2(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
                jump(cJumpHeight, cSpeed);
                isJumping = true;
            }

            // If we have reached our goal on the horizontal plane, terminate movement
            if(centerX >= target.position.x - target.radius && centerX <= target.position.x + target.radius)
            {
                hasActiveTargetArea = false;
                dampenedJump();
            }
        }
    }
}

void Box::moveToTarget(float x, float y, float radius, bool withStopCondition, float speed)
{
    // Current position
    float centerX = body->GetPosition().x * SCALE;
    float centerY = body->GetPosition().y * SCALE;

    float distanceX = x - centerX;
    float distanceY = y - centerY;
    float distance = sqrt(distanceX*distanceX + distanceY*distanceY);
    float mod = speed / distance;
    float targetSpeedX = distanceX * mod;
    float targetSpeedY = distanceY * mod;

    // Stop condition
    if(withStopCondition && centerX <= x + radius && centerX >= x - radius && centerY <= y + radius && centerY >= y - radius)
    {
        hasActiveTargetArea = false;
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    }
    else
    {
        body->SetLinearVelocity(b2Vec2(targetSpeedX, targetSpeedY));
    }
}

void Box::jump(float jump, float speed)
{
    int rndS = rand() % 200 + 900;
    int rndH = rand() % 200 + 900;

    float rndSpeed = rndS / 1000.0f;
    float rndHeight = rndH / 1000.0f;

    b2Vec2 force(speed * rndSpeed, -jump * rndHeight);
    body->SetLinearVelocity(force);
}

void Box::dampenedJump()
{
    float verticalSpeed = body->GetLinearVelocity().y;
    float horizontalSpeed = stopSpeed;

    if(body->GetLinearVelocity().x < 0)
    {
        horizontalSpeed = -horizontalSpeed;
    }

    body->SetLinearVelocity(b2Vec2(horizontalSpeed, verticalSpeed));
}

void Box::setLatestImpulse(float imp)
{
    lastImpulseReceived = imp;
}

void Box::processImpulse()
{
    if(lastImpulseReceived >= IMPULSE_EFFECT_THRESHOLD)
    {
        // Dynamic logic
        if(body->GetType() == b2_dynamicBody)
        {
            //std::cout << "Dynamic Force received: " << lastImpulseReceived << std::endl;
        }
        else // Static logic
        {
            //std::cout << "Static Force received: " << lastImpulseReceived << std::endl;

            // Lose health
            //health -= (int)(lastImpulseReceived * IMPULSE_DAMAGE_MULTIPLIER);

//            if(health <= 0)
//            {
//                isDead = true;
//            }
        }

        // Reset impulse
        lastImpulseReceived = 0.0f;
    }
}

bool Box::isInMotion()
{
    if(body->GetLinearVelocity().x == 0.0f && body->GetLinearVelocity().y == 0.0f)
    {
        return false;
    }

    return true;
}

void Box::attack()
{
    if(repeatAttack && attackTimer.timeReached())
    {
        isAttacking = true;
    }

    if(isAttacking)
    {
        attackTimer.resetTimer();

        // If we are stuck with another box
        if(stickyMaster != NULL)
        {
            // If the other box is not from our team, we cannot attack
            if(stickyMaster->team != team)
            {
                return;
            }
        }

        hasActiveTargetArea = false;

        if(!attackStarted)
        {
            if(contactStaticList.size() > 0 || contactDynamicList.size())
            {
                body->ApplyForceToCenter(b2Vec2(0.0f, -stompJumpForce));
            }
            attackStarted = true;
        }
        else
        {
            if(body->GetLinearVelocity().y >= -1.0f && body->GetLinearVelocity().y <= 1.0f)
            {
                body->ApplyForceToCenter(b2Vec2(0.0f, stompForce));
            }

            if(body->GetLinearVelocity().y > 0.0f)
            {
                for(unsigned int i = 0; i < contactDynamicList.size(); i++)
                {
                    if(contactDynamicList.at(i)->team != team)
                    {
                        contactDynamicList.at(i)->takeHit(attackDamage, team);

                        attackStarted = false;
                        isAttacking = false;
                    }
                }

                for(unsigned int i = 0; i < contactStaticList.size(); i++)
                {
                    if(contactStaticList.at(i)->team != team)
                    {
                        contactStaticList.at(i)->takeHit(attackDamage, team);

                        attackStarted = false;
                        isAttacking = false;
                    }
                }

                if(attackStarted && isAttacking)
                {
                    if(contactDynamicList.size() > 0)
                    {
                        attackStarted = false;
                        isAttacking = false;
                    }
                }

                if(!attackStarted && !isAttacking)
                {
                    playRandomAttackSound();
                }
            }
        }
    }

    if(repeatAttack)
    {
        attackTimer.update();
    }
}

void Box::changeTeam(int team)
{
    this->team = team;
    isActive = false;
    resetAiTarget();
    resetAttack();
    updateTexture();
}

void Box::checkHeight()
{
    if(body->GetPosition().y * SCALE > GROUND_STARTING_HEIGHT + MAX_WORLD_HEIGHT * BOX_SIZE - BOX_SIZE*3)
    {
        repeatAttack = false;
    }

    if((body->GetPosition().y * SCALE > GROUND_STARTING_HEIGHT + MAX_WORLD_HEIGHT * BOX_SIZE) ||
        body->GetPosition().y * SCALE < -MAX_SKY_HEIGHT)
    {
        isDead = true;
    }
}

void Box::notifyNeighbours()
{
    // Iterate through all neighbours
    for(unsigned int i = 0; i < 9; i++)
    {
        // Ignore the center box because that is the one about to be deleted
        if(i == 4)
        {
            continue;
        }

        // Make sure the neighbour exists
        if(neighbours[i].exists)
        {
            // Iterate through the neighbours of the existing neighbour
            for(unsigned int j = 0; j < 9; j++)
            {
                // Skip the center box
                if(j == 4)
                {
                    continue;
                }

                // Make sure the neighbour of the neighbour exists
                if(groundBoxes->at(neighbours[i].index)->neighbours[j].exists)
                {
                    // Check if the neighbour of our neighbour is our original starting point
                    if(groundBoxes->at(neighbours[i].index)->neighbours[j].index == neighbours[4].index)
                    {
                        // Make sure our starting point is marked as non-existant in all neighbours neighbourhoods
                        groundBoxes->at(neighbours[i].index)->neighbours[j].exists = false;
                    }
                }
            }

            // Notify all existing neighbours
            groundBoxes->at(neighbours[i].index)->isNotified = true;
        }
    }
}

void Box::checkNotifications()
{
    if(isNotified)
    {
        int empty = 0;

        for(int i = 0; i < 9; i++)
        {
            if(neighbours[i].exists)
            {
                empty++;
            }
        }

        if(empty > 0)
        {
            setBodyActive(true);
        }

        isNotified = false;
    }
}

void Box::checkForConversion()
{
    bool top = neighbours[1].exists;
    bool left = neighbours[3].exists;
    bool bottom = neighbours[5].exists;
    bool right = neighbours[7].exists;

    if(!top && !left && !bottom && !right)
    {
        isFlaggedForConversion = true;
    }
}

void Box::reduceCharges(int amount)
{
    tagCharges -= amount;

    if(tagCharges < 0)
    {
        tagCharges = 0;
    }
}


void Box::displayAdditionals(sf::RenderWindow& window)
{
    displayMarker(window);
    displayInfo(window);

    if(!hitNumberTimer.timeReached())
    {
        float posX = body->GetPosition().x * SCALE -10;
        float posY = body->GetPosition().y * SCALE - BOX_SIZE/2 -15;
        text_damageTaken.setPosition(posX, posY);
        window.draw(text_damageTaken);
    }
}

void Box::displayMarker(sf::RenderWindow& window)
{
    if(team == TEAM_ALLY && isActive)
    {
        sf::Vector2f a(body->GetPosition().x * SCALE - 5, body->GetPosition().y * SCALE - BOX_SIZE/2.0f - 13);
        sf::Vector2f b(body->GetPosition().x * SCALE + 5, body->GetPosition().y * SCALE - BOX_SIZE/2.0f - 13);
        sf::Vector2f c(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE - BOX_SIZE/2.0f - 5);
        sf::Vertex line[3] = {a, b, c};

        line[0].color = sf::Color::Green;
        line[1].color = sf::Color::Green;
        line[2].color = sf::Color::Green;

        window.draw(line, 3, sf::Triangles);
    }

//    if(hasActiveTargetArea)
//    {
//        float centerX = body->GetPosition().x * SCALE;
//        float centerY = body->GetPosition().y * SCALE;
//
//        float distX = target.position.x - centerX;
//        float distY = target.position.y - centerY;
//        float dist = sqrt(distX*distX + distY*distY);
//        float mod = (BOX_SIZE) / dist;
//
//        float lengthX = distX * mod;
//        float lengthY = distY * mod;
//
//        sf::Vector2f a(centerX, centerY);
//        sf::Vector2f b(centerX + lengthX, centerY + lengthY);
//
//        sf::Vertex line[2] = {a, b};
//
//        line[0].color = sf::Color::White;
//        line[1].color = sf::Color::White;
//
//        window.draw(line, 2, sf::Lines);
//    }
}

void Box::displayInfo(sf::RenderWindow& window)
{
    if(global_showFriendlyBoxInfo || isInfo)
    {
        updateInfo();

        window.draw(text_background);

        for(unsigned int i = 0; i < text_info.size(); i++)
        {
            window.draw(text_info.at(i));
        }
    }
}

void Box::addBoxInfo(int id, std::string text)
{
    // Position
    calcInfoBoxPosition();
    float posX = text_topLeftX;
    float posY = text_topLeftY;

    // Type
    text_info.at(id).setString(text);
    text_info.at(id).setPosition(posX, posY + id * DEFAULT_LINE_HEIGHT);
}

void Box::updateInfo()
{
    std::stringstream stream_type;
    stream_type << "Type: " << getProperType(type);
    std::stringstream stream_charges;
    stream_charges << "Charges: " << tagCharges;

    std::string string_team;

    if(team == TEAM_NEUTRAL)
    {
        text_info.at(0).setColor(sf::Color::White);
        string_team = "Neutral";
    }
    else if(team == TEAM_ALLY)
    {
        text_info.at(0).setColor(sf::Color::Green);
        string_team = "Ally";
    }
    else if(team == TEAM_ENEMY)
    {
        text_info.at(0).setColor(sf::Color::Red);
        string_team = "Enemy";
    }

    std::stringstream stream_health;
    stream_health << "Health: " << health;

    // Position
    calcInfoBoxPosition();
    float posX = text_topLeftX;
    float posY = text_topLeftY;

    // Background
    text_background.setFillColor(sf::Color(0, 0, 0, 155));
    text_background.setPosition(posX - DEFAULT_TEXT_BOX_PADDING, posY - DEFAULT_TEXT_BOX_PADDING);
    text_background.setSize(sf::Vector2f((posX + BOX_SIZE*3.0f + DEFAULT_TEXT_BOX_PADDING) - (posX - DEFAULT_TEXT_BOX_PADDING), (posY + DEFAULT_LINE_HEIGHT * text_info.size() + DEFAULT_TEXT_BOX_PADDING) - (posY - DEFAULT_TEXT_BOX_PADDING)));

    // Team
    text_info.at(0).setString(string_team);
    text_info.at(0).setPosition(posX, posY);
    text_info.at(0).setStyle(sf::Text::Bold);

    // Type
    addBoxInfo(1, stream_type.str());

    // Charges
    addBoxInfo(2, stream_charges.str());

    // Health
    addBoxInfo(3, stream_health.str());
}

int Box::addBoxInfoLine()
{
    text_info.push_back(sf::Text("", font_default, DEFAULT_TEXT_SIZE));
    text_info.at(text_info.size() - 1).setColor(sf::Color::White);

    return text_info.size() - 1;
}

void Box::calcInfoBoxPosition()
{
    text_topLeftX = body->GetPosition().x * SCALE - BOX_SIZE/0.7f;
    text_topLeftY = body->GetPosition().y * SCALE - BOX_SIZE/1.7f - (text_info.size()) * DEFAULT_LINE_HEIGHT - DEFAULT_BOX_INFO_DISTANCE;
}

void Box::notifyContactsOnConversion(bool staticToDynamic)
{
    // ------------------------------- Conversion from static to dynamic -------------------------------
    if(staticToDynamic)
    {
        // All dynamic contacts remove this from their static list and push this into the dynamic list
        for(unsigned int i = 0; i < contactDynamicList.size(); i++)
        {
            contactDynamicList.at(i)->contactStaticList.erase(std::remove(contactDynamicList.at(i)->contactStaticList.begin(), contactDynamicList.at(i)->contactStaticList.end(), this), contactDynamicList.at(i)->contactStaticList.end());
            contactDynamicList.at(i)->contactDynamicList.push_back(this);
        }

        // All static contacts remove this from their static list and push this into the dynamic list
        for(unsigned int i = 0; i < contactStaticList.size(); i++)
        {
            contactStaticList.at(i)->contactStaticList.erase(std::remove(contactStaticList.at(i)->contactStaticList.begin(), contactStaticList.at(i)->contactStaticList.end(), this), contactStaticList.at(i)->contactStaticList.end());
            contactStaticList.at(i)->contactDynamicList.push_back(this);
        }
    }
    else // ------------------------------- Conversion from dynamic to static -------------------------------
    {
        // All dynamic contacts remove this from their dynamic list and push this into the static list
        for(unsigned int i = 0; i < contactDynamicList.size(); i++)
        {
            contactDynamicList.at(i)->contactDynamicList.erase(std::remove(contactDynamicList.at(i)->contactDynamicList.begin(), contactDynamicList.at(i)->contactDynamicList.end(), this), contactDynamicList.at(i)->contactDynamicList.end());
            contactDynamicList.at(i)->contactStaticList.push_back(this);
        }

        // All static contacts remove this from their dynamic list and push this into the static list
        for(unsigned int i = 0; i < contactStaticList.size(); i++)
        {
            contactStaticList.at(i)->contactDynamicList.erase(std::remove(contactStaticList.at(i)->contactDynamicList.begin(), contactStaticList.at(i)->contactDynamicList.end(), this), contactStaticList.at(i)->contactDynamicList.end());
            contactStaticList.at(i)->contactStaticList.push_back(this);
        }
    }

}

void Box::setBodyActive(bool active)
{
    body->SetActive(active);

    updateTexture();
}

Box* Box::getDynamicBoxByCoordinates(int x, int y)
{
    for(unsigned int i = 0; i < boxes->size(); i++)
    {
        float minX = boxes->at(i)->getBody()->GetPosition().x * SCALE - BOX_SIZE / 2.0f;
        float maxX = minX + BOX_SIZE;
        float minY = boxes->at(i)->getBody()->GetPosition().y * SCALE - BOX_SIZE / 2.0f;
        float maxY = minY + BOX_SIZE;

        if(x >= minX && x <= maxX && y >= minY && y <= maxY)
        {
            return boxes->at(i);
        }
    }

    return NULL;
}

void Box::ai()
{
    if(stickyMaster == NULL)
    {
        for(unsigned int i = 0; i < contactDynamicList.size(); i++)
        {
            if(contactDynamicList.at(i)->type == "floatbox" && contactDynamicList.at(i)->isSticky && type != "floatbox")
            {
                //moveToTarget(contactDynamicList.at(i)->getBody()->GetPosition().x * SCALE, contactDynamicList.at(i)->getBody()->GetPosition().y * SCALE, 0, false, 4.0f);
                stickyMaster = contactDynamicList.at(i);
                body->SetGravityScale(0.0f);
            }
        }
    }

    if(stickyMaster != NULL)
    {
        body->SetLinearVelocity(stickyMaster->getBody()->GetLinearVelocity());

        float centerX = body->GetPosition().x * SCALE;
        float centerY = body->GetPosition().y * SCALE;
        float stickyX = stickyMaster->getBody()->GetPosition().x * SCALE;
        float stickyY = stickyMaster->getBody()->GetPosition().y * SCALE;
        float distX = stickyX - centerX;
        float distY = stickyY - centerY;
        float dist = sqrt(distX*distX + distY*distY);

        if(!stickyMaster->isSticky || dist > BOX_SIZE + 3.0f)
        {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            body->SetGravityScale(1.0f);
            stickyMaster = NULL;
        }
    }

    if((team == TEAM_ENEMY && type != "floatbox") || (team == TEAM_ALLY && type != "floatbox" && type != "chargebox" && !isActive && global_enableAllyAI))
    {
        if(aiAttackTimer.timeReached())
        {
            if(aiTarget == NULL)
            {
                int minX = body->GetPosition().x * SCALE - BOX_SIZE/2 - attackRange;
                int minY = body->GetPosition().y * SCALE - BOX_SIZE/2 - attackRange;
                int maxX = minX + 2 * attackRange;
                int maxY = minY + 2 * attackRange;

                for(unsigned int i = 0; i < boxes->size(); i++)
                {
                    if(boxes->at(i)->team != team && boxes->at(i)->team != TEAM_NEUTRAL) //&& boxes->at(i)->targetMasters.size() < rand() % 3 + 2)
                    {
                        if(rand() % 100 < 50)
                        {
                            continue;
                        }

                        int boxX = boxes->at(i)->getBody()->GetPosition().x * SCALE;
                        int boxY = boxes->at(i)->getBody()->GetPosition().y * SCALE;

                        if(boxX >= minX && boxX <= maxX && boxY >= minY && boxY <= maxY)
                        {
                            aiTarget = boxes->at(i);
                            boxes->at(i)->targetMasters.push_back(this);
                            break;
                        }
                    }
                }
            }
            else
            {
                if(aiTarget != NULL)
                {
                    target = TargetArea(aiTarget->getBody()->GetPosition().x * SCALE, aiTarget->getBody()->GetPosition().y * SCALE - BOX_SIZE, BOX_SIZE/4.0f);
                    hasActiveTargetArea = true;
                }

                if(body->GetPosition().x * SCALE >= target.position.x - target.radius && body->GetPosition().x * SCALE <= target.position.x + target.radius)
                {
                    if(type != "magicbox")
                    {
                        isAttacking = true;
                    }

                    hasActiveTargetArea = false;
                    aiTarget->targetMasters.erase(std::remove(aiTarget->targetMasters.begin(), aiTarget->targetMasters.end(), this), aiTarget->targetMasters.end());
                    aiTarget = NULL;
                    aiAttackTimer.resetTimer();
                }
            }
        }

        aiAttackTimer.update();
    }
}

void Box::takeHit(float dmg, int team)
{
    // Last damage
    lastDamageReceivedFrom = team;

    // Calc damage
    if(damageResistance > 0)
    {
        health -= dmg * (1/damageResistance);

        // Reset timer for flicker
        hitTimer.resetTimer();

        // And show damage number
        hitNumberTimer.resetTimer();

        // Fill damage text with info
        std::stringstream dmgNumber;
        dmgNumber << "-" << (int)(dmg * (1/damageResistance));
        text_damageTaken.setString(dmgNumber.str());
    }
    else // Instagib without resistance
    {
        isDead = true;
    }

    // Die if helath too low
    if(health <= 0)
    {
        isDead = true;
    }
}

void Box::notifyDynamics()
{
    if(stickyMaster != NULL)
    {
        stickyMaster->resetAiTarget();
    }

    if(floatMaster != NULL)
    {
        floatMaster->resetAiTarget();
    }

    if(targetMasters.size() > 0)
    {
        for(unsigned int i = 0; i < targetMasters.size(); i++)
        {
            targetMasters.at(i)->resetAiTarget();
        }
    }
}

void Box::resetAiTarget()
{
    aiTarget = NULL;
}

void Box::heal(float health)
{
    this->health += health;

    if(this->health > 100)
    {
        this->health = 100;
    }
}

void Box::resetAttack()
{
    repeatAttack = false;
    isAttacking = false;
    attackStarted = false;
}

void Box::stop()
{
    resetAttack();
    hasActiveTargetArea = false;
    body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
}

void Box::playRandomAttackSound()
{
    int rnd = rand() % 100 + 1;

    if(rnd <= 50)
    {
        box_attack.play();
    }
    else
    {
        box_attack.play();
    }
}
