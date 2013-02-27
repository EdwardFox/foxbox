#include "ChargeBox.h"

ChargeBox::ChargeBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes) : Box(groundBoxes, boxes)
{
    init();
}

ChargeBox::~ChargeBox()
{
    //dtor
}

void ChargeBox::init()
{
    type = "chargebox";
    tagCharges = 1;
    attackReady = true;
    attackStarted = false;
    attackStopped = false;
    speed = 50000.0f;
    rechargeTimer.setThreshold(5.0f);
    attackDamage = 50;
    damageResistance = 1.1f;
    dynamicTargetHit = false;

    text_id_attackReady = addBoxInfoLine();
}

void ChargeBox::updateInfo()
{
    Box::updateInfo();

    std::string string_attack;

    if(attackReady)
    {
        string_attack = "Ready";
    }
    else
    {
        std::stringstream stream_attack;
        stream_attack << "Ready In: " << (int)rechargeTimer.getTimeLeft();
        string_attack = stream_attack.str();
    }

    addBoxInfo(text_id_attackReady, string_attack);
}

void ChargeBox::ai()
{
    // Default AI
    Box::ai();

    if(isConverted && team == TEAM_NEUTRAL)
    {
        conversionTarget.x = body->GetPosition().x * SCALE + (rand() % 200 - 101);
        conversionTarget.y = body->GetPosition().y * SCALE + (rand() % 200 - 151);

        isAttacking = true;
    }
}

void ChargeBox::attack()
{
    if(isAttacking && hasActiveTargetArea)
    {
        isAttacking = false;
    }

    if(attackReady && isAttacking && !attackStarted && !hasActiveTargetArea && (contactStaticList.size() > 0 || contactDynamicList.size() > 0))
    {
        if(isConverted && team == TEAM_NEUTRAL)
        {
            lastMouse.x = conversionTarget.x;
            lastMouse.y = conversionTarget.y;

            body->SetAngularVelocity(rand() % 31 - 15);

            isConverted = false;
        }

        float distX = lastMouse.x - body->GetPosition().x * SCALE;
        float distY = lastMouse.y - body->GetPosition().y * SCALE;
        float distance = sqrt(distX*distX + distY*distY);
        float mod = speed / distance;
        targetSpeed.x = distX * mod;
        targetSpeed.y = distY * mod;

        body->ApplyForceToCenter(targetSpeed);

        attackStarted = true;
        isAttacking = false;
        attackReady = false;
    }
    else if(attackStarted)
    {
        if(!dynamicTargetHit)
        {
            for(unsigned int i = 0; i < contactDynamicList.size(); i++)
            {
                if(contactDynamicList.at(i)->team != team)
                {
                    contactDynamicList.at(i)->takeHit(attackDamage, team);
                    dynamicTargetHit = true;
                }
            }
        }

        // Stop condition
        if((contactStaticList.size() > 0 && rechargeTimer.timeReached()) || (contactDynamicList.size() > 0 && rechargeTimer.timeReached()))
        {
            attackStarted = false;
            isAttacking = false;
            attackReady = true;
            attackStopped = false;
            dynamicTargetHit = false;
            rechargeTimer.resetTimer();
        }
    }

    // Recharge timer
    if(isAttacking || attackStarted)
    {
        rechargeTimer.update();
    }
}
