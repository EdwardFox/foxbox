#include "MagicBox.h"

MagicBox::MagicBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes) : Box(groundBoxes, boxes)
{
    init();
}

MagicBox::~MagicBox()
{
    //dtor
}

void MagicBox::init()
{
    type = "magicbox";
    tagCharges = 0;
    attackDamage = 0;
    damageResistance = 0.8f;
    healPower = 40;
    squareRadius = 1;
}

void MagicBox::ai()
{
    // Default AI
    Box::ai();

    // Coming out of conversion
    if(isConverted && team == TEAM_NEUTRAL)
    {
        bool isLookingForDestination = true;

        if(contactStaticList.size() > 0)
        {
            while(isLookingForDestination)
            {
                b2Vec2 pos = getGroundCoordsByCoordinates(rand() % (MAX_WORLD_WIDTH * (int)BOX_SIZE),rand() % 400 - 100);

                std::stringstream id;
                id << pos.x << "x" << pos.y;

                if(groundBoxes->find(id.str()) == groundBoxes->end())
                {
                    body->SetTransform(b2Vec2(pos.x/SCALE, pos.y/SCALE), 0.0f);
                    isLookingForDestination = false;
                }
            }

            isConverted = false;
        }
    }

    // Attack if touching enemy team
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

void MagicBox::attack()
{
    if(isAttacking)
    {
        int minX = body->GetPosition().x * SCALE - squareRadius * BOX_SIZE - BOX_SIZE / 2;
        int minY = body->GetPosition().y * SCALE - squareRadius * BOX_SIZE - BOX_SIZE / 2;
        int maxX = minX + (1 + 2 * squareRadius) * BOX_SIZE;
        int maxY = minY + (1 + 2 * squareRadius) * BOX_SIZE;

        // Dynamic boxes
        for(unsigned int i = 0; i < boxes->size(); i++)
        {
            int dynX = boxes->at(i)->getBody()->GetPosition().x * SCALE;
            int dynY = boxes->at(i)->getBody()->GetPosition().y * SCALE;

            if(dynX >= minX && dynX <= maxX && dynY >= minY && dynY <= maxY)
            {
                // Heal allies
                if(boxes->at(i)->team == team)
                {
                    // Heal ignores resistances, no overheal
                    boxes->at(i)->heal(healPower);
                }

                if(boxes->at(i)->team == TEAM_NEUTRAL)
                {
                    boxes->at(i)->changeTeam(team);
                }

                if(boxes->at(i)->team != team)
                {
                    boxes->at(i)->changeTeam(TEAM_NEUTRAL);
                }
            }
        }

        b2Vec2 pos = getGroundCoordsByCoordinates(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        neighbour neighbours[9];
        int index = 0;

        // Find static boxes within radius
        for(int i = -squareRadius; i <= squareRadius; i++)
        {
            for(int j = -squareRadius; j <= squareRadius; j++)
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
                    groundBoxes->at(neighbours[index].index)->isFlaggedForConversion = true;
                }

                index++;
            }
        }

        isAttacking = false;
        isDead = true;
    }
}
