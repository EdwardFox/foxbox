#include "FoxBox.h"

FoxBox::FoxBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes) : Box(groundBoxes, boxes)
{
    init();
}

FoxBox::~FoxBox()
{
    //dtor
}

void FoxBox::init()
{
    type = "foxbox";
    tagCharges = 0;
    health = 9999;
}

void FoxBox::ai()
{
    body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    body->SetGravityScale(0.0f);

    for(unsigned int i = 0; i < contactDynamicList.size(); i++)
    {
        if(contactDynamicList.at(i)->team == TEAM_ALLY && contactDynamicList.at(i)->type != "floatbox")
        {
            global_levelComplete = true;
            isDead = true;
        }
    }
}

void FoxBox::changeTeam(int team)
{
    updateTexture();
}
