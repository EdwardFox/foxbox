#include "BoomBox.h"

BoomBox::BoomBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes) : Box(groundBoxes, boxes)
{
    init();
}

BoomBox::~BoomBox()
{
    //dtor
}

void BoomBox::init()
{
    type = "boombox";
    radius = 3;
    powerMod = 5000.0f;
    rotationMod = 2.0f;
    detonateOnContact = false;
    tagCharges = 0;
    attackDamage = 50;
    damageResistance = 0.75f;

    radius = rand() % 3 + 3;

    text_id_radius = addBoxInfoLine();
}

void BoomBox::updateInfo()
{
    Box::updateInfo();

    std::stringstream stream_radius;
    stream_radius << "Radius: " << radius;

    addBoxInfo(text_id_radius, stream_radius.str());
}

void BoomBox::ai()
{
    // Default AI
    Box::ai();

    // Explode if last impulse was too great
    // A little buggy, impulses are acting weird somtimes
    if(team == TEAM_NEUTRAL && isConverted)
    {
        if(lastImpulseReceived > 230.0f)
        {
            isDead = true;
            isConverted = false;
            lastImpulseReceived = 0.0f;
        }
    }

    // Explode if touching allied team
    if(team != TEAM_NEUTRAL)
    {
        for(unsigned int i = 0; i < contactDynamicList.size(); i++)
        {
            if(contactDynamicList.at(i)->team != team && contactDynamicList.at(i)->team != TEAM_NEUTRAL)
            {
                isAttacking = true;
            }
        }
    }
}

void BoomBox::attack()
{
    if((contactStaticList.size() > 0 && detonateOnContact) || (debug_global_detonateOnContact && contactStaticList.size() > 0) || isDead)
    {
        isAttacking = true;
    }

    if(isAttacking)
    {
        int amount = (4*radius*radius + 4*radius)/2 + 1;
        int index = 0;
        int perRow = 0;
        neighbour neighbours[amount];

        b2Vec2 pos = getGroundCoordsByCoordinates(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);

        // Find static boxes within radius
        for(int i = -radius; i <= radius; i++)
        {
            for(int j = -radius; j <= radius; j++)
            {
                // Boxes per row
                perRow = 0;
                if(i <= 0)
                {
                    perRow = (i + radius)*2 + 1;
                }
                else
                {
                    perRow = (-i + radius)*2 + 1;
                }

                perRow /= 2;

                // Fill neighbours
                if(j >= -perRow && j <= perRow)
                {
                    neighbours[index].x = pos.x + j * BOX_SIZE;
                    neighbours[index].y = pos.y + i * BOX_SIZE;

                    std::stringstream id;
                    id << neighbours[index].x << "x" << neighbours[index].y;
                    neighbours[index].index = id.str();

                    if(groundBoxes->find(id.str()) == groundBoxes->end())
                    {
                        neighbours[index].exists = false;
                    }
                    else
                    {
                        neighbours[index].exists = true;
                    }

                    // Set dead if neighbour exists
                    if(neighbours[index].exists)
                    {
                        groundBoxes->at(neighbours[index].index)->isDead = true;
                    }

                    index++;
                }
            }
        }

        for(unsigned int i = 0; i < boxes->size(); i++)
        {
            int x = boxes->at(i)->getBody()->GetPosition().x * SCALE;
            int y = boxes->at(i)->getBody()->GetPosition().y * SCALE;

            if(x <= pos.x + radius * BOX_SIZE && x >= pos.x - radius * BOX_SIZE &&
               y <= pos.y + radius * BOX_SIZE && y >= pos.y - radius * BOX_SIZE)
               {
                    float distX = x - pos.x;
                    float distY = y - pos.y;
                    float distance = sqrt(distX*distX + distY*distY);

                    float forceX = (distX / distance) * radius * powerMod;
                    float forceY = (distY / distance) * radius * powerMod;

                    b2Vec2 force(forceX, forceY);

                    boxes->at(i)->getBody()->ApplyForceToCenter(force);

                    if(distX < 0)
                    {
                        boxes->at(i)->getBody()->ApplyTorque(-powerMod/2);
                    }
                    else
                    {
                        boxes->at(i)->getBody()->ApplyTorque(powerMod/2);
                    }

                    if(boxes->at(i)->team != team)
                    {
                        boxes->at(i)->takeHit(attackDamage, team);
                    }
               }
        }

        isAttacking = false;
        isDead = true;
    }
}
