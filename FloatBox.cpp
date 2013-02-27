#include "FloatBox.h"

FloatBox::FloatBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes) : Box(groundBoxes, boxes)
{
    init();
}

FloatBox::~FloatBox()
{
    //dtor
}

void FloatBox::init()
{
    type = "floatbox";
    tagCharges = 1;
    floatNeutralSpeed = 2.0f;
    speed = 3.0f;
    isSticky = false;
    text_id_sticky = addBoxInfoLine();
    attackDamage = 0;
    damageResistance = 1.2f;

    // AI
    searchRange = 400;
    searchRangeVertical = 3000;
    hasGainedAltitude = false;
    hasGainedAltitudeAgain = false;
    hasTargetCaptured = false;
}

void FloatBox::createBox(b2World& world, b2Vec2 position, b2Vec2 size, float density, float friction, bool isDynamic, uint16 categoryBits, uint16 maskBits)
{
    Box::createBox(world, position, size, density, friction, isDynamic, categoryBits, maskBits);

    body->SetGravityScale(0.0f);
}

void FloatBox::updateInfo()
{
    Box::updateInfo();

    std::string string_sticky;

    if(isSticky)
    {
        string_sticky = "Sticky";
    }
    else
    {
        string_sticky = "Not Sticky";
    }

    addBoxInfo(text_id_sticky, string_sticky);
}

void FloatBox::ai()
{
    Box::ai();

    // Active
    if(body->IsActive())
    {
        if(team == TEAM_NEUTRAL)
        {
            body->SetLinearVelocity(b2Vec2(0.0f, -floatNeutralSpeed));
        }
        else if(team == TEAM_ALLY && !hasActiveTargetArea && isInMotion())
        {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            body->SetAngularVelocity(0.0f);
        }
        else if(team == TEAM_ENEMY)
        {
            // Do not rotate (fucks up capturing algorithm)
            if(!body->IsFixedRotation())
            {
                body->SetFixedRotation(true);
            }

            // Do we have a target?
            if(aiTarget == NULL)
            {
                // Do not move without target
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

                // Find target
                for(unsigned int i = 0; i < boxes->size(); i++)
                {
                    // Team ally only and NOT the foxbox or other floatboxes
                    if(boxes->at(i)->team == TEAM_ALLY && boxes->at(i)->type != "foxbox" && boxes->at(i)->type != "floatbox")
                    {
                        float centerX = body->GetPosition().x * SCALE;
                        float centerY = body->GetPosition().y * SCALE;
                        float targetX = boxes->at(i)->getBody()->GetPosition().x * SCALE;
                        float targetY = boxes->at(i)->getBody()->GetPosition().y * SCALE;

                        // ... within range
                        if(targetX >= centerX - searchRange && targetX <= centerX + searchRange && targetY >= centerY - searchRangeVertical && targetY <= centerY + searchRangeVertical)
                        {
                            // ... and has not already been targeted
                            if(boxes->at(i)->floatMaster == NULL)
                            {
                                aiTarget = boxes->at(i);
                                aiTarget->floatMaster = this;
                                isSticky = true;
                                returnPos.x = rand() % MAX_WORLD_WIDTH * BOX_SIZE;
                                returnPos.y = rand() % 2500 - 2500;
                                firstRandomAltitude = rand() % 400 - 400;
                                secondRandomAltitude = rand() % 400 - 400;
                                dropRange = rand() % 400 - 500;
                                break;
                            }

                        }
                    }
                }
            }
            else // We have a target in sight
            {
                // Gain proper altitude
                if(!hasGainedAltitude)
                {
                    body->SetLinearVelocity(b2Vec2(0.0f, -speed));

                    if(body->GetPosition().y * SCALE <= firstRandomAltitude)
                    {
                        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                        hasGainedAltitude = true;
                    }
                }
                else // We have our desired altitude
                {
                    // Target captured?
                    if(!hasTargetCaptured)
                    {
                        float centerX = body->GetPosition().x * SCALE;
                        float targetX = aiTarget->getBody()->GetPosition().x * SCALE;

                        // Only move down to target if we are directly above it
                        if(centerX >= targetX - 1.0f && centerX <= targetX + 1.0f)
                        {
                            moveToTarget(aiTarget->getBody()->GetPosition().x * SCALE, aiTarget->getBody()->GetPosition().y * SCALE, 1.0f, false, speed);

                            // Target captured?
                            if(contactDynamicList.size() > 0)
                            {
                                // ... only if opposing team
                                if(contactDynamicList.at(0)->team == TEAM_ALLY)
                                {
                                    hasTargetCaptured = true;
                                }
                                else // Move away if collision with friendly unit
                                {
                                    body->SetLinearVelocity(b2Vec2(-speed, -speed));
                                }

                            }
                        }
                        else // We are not directly above it; adjust horizontal position
                        {
                            float cSpeed = speed;

                            if(targetX < centerX)
                            {
                                cSpeed = -cSpeed;
                            }

                            body->SetLinearVelocity(b2Vec2(cSpeed, 0.0f));
                        }

                        // Shake free if stuck with another floater
                        if(contactDynamicList.size() > 0 && contactDynamicList.at(0)->team == TEAM_ENEMY && contactDynamicList.at(0)->type == "floatbox")
                        {
                            int cSpeed = speed;
                            int uSpeed = speed;

                            if(rand() % 100 < 50)
                            {
                                cSpeed = -cSpeed;
                            }

                            if(rand() % 100 < 50)
                            {
                                uSpeed = -uSpeed;
                            }


                            body->SetLinearVelocity(b2Vec2(cSpeed, uSpeed));
                        }
                    }
                    else // We have the target captured
                    {
                        // Gain altitude again
                        if(!hasGainedAltitudeAgain)
                        {
                            body->SetLinearVelocity(b2Vec2(0.0f, -speed));

                            if(body->GetPosition().y * SCALE <= secondRandomAltitude)
                            {
                                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                                hasGainedAltitudeAgain = true;
                            }
                        }
                        else // Altitude gained
                        {

                            // Still carrying target
                            if(isSticky)
                            {
                                body->SetLinearVelocity(b2Vec2(-speed, 0.0f));

                                // Drop target into nothingness
                                if(body->GetPosition().x * SCALE <= dropRange)
                                {
                                    isSticky = false;
                                }
                            }
                            else // Target dropped
                            {
                                // Return to random altitude
                                if(body->GetPosition().y * SCALE <= returnPos.y)
                                {
                                    body->SetLinearVelocity(b2Vec2(speed, 0.0f));
                                }
                                else
                                {
                                    body->SetLinearVelocity(b2Vec2(speed, -speed));
                                }

                                // Shake free if stuck with another floater
                                if(contactDynamicList.size() > 0 && contactDynamicList.at(0)->team == TEAM_ENEMY  && contactDynamicList.at(0)->type == "floatbox")
                                {
                                    int cSpeed = speed;

                                    if(rand() % 100 < 50)
                                    {
                                        cSpeed = -cSpeed;
                                    }

                                    body->SetLinearVelocity(b2Vec2(cSpeed, -speed));
                                }

                                // Returned to random horizontal position
                                if(body->GetPosition().x * SCALE >= returnPos.x)
                                {
                                    std::cout << "target freed" << std::endl;
                                    aiTarget->floatMaster = NULL;
                                    aiTarget = NULL;
                                    hasGainedAltitude = false;
                                    hasGainedAltitudeAgain = false;
                                    hasTargetCaptured = false;
                                    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                                }
                            }
                        }
                    }
                }

                if(hasTargetCaptured && (aiTarget->stickyMaster == NULL || aiTarget == NULL))
                {
                    hasGainedAltitudeAgain = true;
                    isSticky = false;
                }
            }
        }
    }
}

void FloatBox::changeTeam(int team)
{
    Box::changeTeam(team);

    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}

void FloatBox::moveToTargetArea()
{
    if(hasActiveTargetArea)
    {
        moveToTarget(target.position.x, target.position.y, target.radius, true, speed);
    }
}

void FloatBox::attack()
{
    if(isAttacking)
    {
        if(!isSticky)
        {
            isSticky = true;
        }
        else
        {
            isSticky = false;
        }

        isAttacking = false;
    }
}
