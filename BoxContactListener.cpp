#include "BoxContactListener.h"

BoxContactListener::BoxContactListener()
{
    //ctor
}

BoxContactListener::~BoxContactListener()
{
    //dtor
}

void BoxContactListener::BeginContact(b2Contact* contact)
{
    Box* box1;
    Box* box2;

    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(bodyUserData)
    {
        box1 = static_cast<Box*>(bodyUserData);

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if(bodyUserData)
        {
            box2 = static_cast<Box*>(bodyUserData);

            // Contact with static body for reset jumping
            if(box1->getBody()->GetType() == b2_dynamicBody && box2->getBody()->GetType() == b2_staticBody)
            {
                box1->startContact(box2, false);
                box2->startContact(box1, true);
            }

            if(box2->getBody()->GetType() == b2_dynamicBody && box1->getBody()->GetType() == b2_staticBody)
            {
                box2->startContact(box1, false);
                box1->startContact(box2, true);
            }

            if(box1->getBody()->GetType() == b2_dynamicBody && box2->getBody()->GetType() == b2_dynamicBody)
            {
                checkTeamConversion(box1, box2);

                box1->startContact(box2, true);
                box2->startContact(box1, true);
            }
        }
    }
}

void BoxContactListener::EndContact(b2Contact* contact)
{
    Box* box1;
    Box* box2;

    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(bodyUserData)
    {
        box1 = static_cast<Box*>(bodyUserData);

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();

        if(bodyUserData)
        {
            box2 = static_cast<Box*>(bodyUserData);

            // Contact with static body for reset jumping
            if(box1->getBody()->GetType() == b2_dynamicBody && box2->getBody()->GetType() == b2_staticBody)
            {
                box1->endContact(box2, false);
                box2->endContact(box1, true);
            }

            if(box2->getBody()->GetType() == b2_dynamicBody && box1->getBody()->GetType() == b2_staticBody)
            {
                box2->endContact(box1, false);
                box1->endContact(box2, true);
            }

            if(box1->getBody()->GetType() == b2_dynamicBody && box2->getBody()->GetType() == b2_dynamicBody)
            {
                // Invoke end contact methods
                box1->endContact(box2, true);
                box2->endContact(box1, true);
            }
        }
    }
}

void BoxContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    Box* box1;
    Box* box2;

    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(bodyUserData)
    {
        box1 = static_cast<Box*>(bodyUserData);

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if(bodyUserData)
        {
            box2 = static_cast<Box*>(bodyUserData);

            box1->setLatestImpulse(impulse->normalImpulses[0]);
            box2->setLatestImpulse(impulse->normalImpulses[1]);
        }
    }
}

void BoxContactListener::checkTeamConversion(Box* box1, Box* box2)
{
    // If a neutral box was touched by a non-neutral box with charges left
    // change team
    if(box1->team == 0 && box2->tagCharges > 0 && box2->team != 0)
    {
        box1->changeTeam(box2->team);
        box2->reduceCharges(1);
    }
    else if(box2->team == 0 && box1->tagCharges > 0 && box1->team != 0)
    {
        box2->changeTeam(box1->team);
        box2->updateTexture();
        box1->reduceCharges(1);
    }
}
