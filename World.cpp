#include "World.h"

World::World()
{
    resourceTimer.setThreshold(10.0f);
}

World::~World()
{
    //dtor
}

bool World::groundBoxExists(const std::string id)
{
    if(groundBoxes.find(id) == groundBoxes.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void World::createGroundBox(b2World& world, float x, float y, float density, float friction, std::string type)
{
    if(type == "boombox")
    {
        BoomBox* box = new BoomBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(x, y), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, false, BOX_STATIC, BOX_DYNAMIC | BOX_GIB | BOX_STATIC);
        box->team = getRandomTeam("boombox");

        std::stringstream index;
        index << x << "x" << y;

        groundBoxes[index.str()] = box;
    }
    else if(type == "chargebox")
    {
        ChargeBox* box = new ChargeBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(x, y), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, false, BOX_STATIC, BOX_DYNAMIC | BOX_GIB | BOX_STATIC);

        std::stringstream index;
        index << x << "x" << y;

        groundBoxes[index.str()] = box;
    }
    else if(type == "floatbox")
    {
        FloatBox* box = new FloatBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(x, y), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, false, BOX_STATIC, BOX_DYNAMIC | BOX_GIB | BOX_STATIC);
        box->team = getRandomTeam("floatbox");

        std::stringstream index;
        index << x << "x" << y;

        groundBoxes[index.str()] = box;
    }
    else if(type == "magicbox")
    {
        MagicBox* box = new MagicBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(x, y), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, false, BOX_STATIC, BOX_DYNAMIC | BOX_GIB | BOX_STATIC);

        std::stringstream index;
        index << x << "x" << y;

        groundBoxes[index.str()] = box;
    }
    else
    {
        Box* box = new Box(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(x, y), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, false, BOX_STATIC, BOX_DYNAMIC | BOX_GIB | BOX_STATIC);

        std::stringstream index;
        index << x << "x" << y;

        groundBoxes[index.str()] = box;
    }
}

void World::createBox(b2World& world, int mouseX, int mouseY, float density, float friction, int team, std::string type)
{
    if(type == "boombox")
    {
        BoomBox* box = new BoomBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
    else if(type == "chargebox")
    {
        ChargeBox* box = new ChargeBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
    else if(type == "floatbox")
    {
        FloatBox* box = new FloatBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
    else if(type == "magicbox")
    {
        MagicBox* box = new MagicBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
    else if(type == "foxbox")
    {
        FoxBox* box = new FoxBox(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
    else
    {
        Box* box = new Box(&groundBoxes, &boxes);
        box->createBox(world, b2Vec2(mouseX, mouseY), b2Vec2(BOX_SIZE, BOX_SIZE), density, friction, true, BOX_DYNAMIC, BOX_STATIC | BOX_DYNAMIC);
        box->changeTeam(team);
        boxes.push_back(box);
    }
}

void World::createGib(b2World& world, int x, int y, sf::Color color, bool isFloating)
{
    Gibs* gib = new Gibs();
    gib->createGib(world, b2Vec2(x, y), b2Vec2(GIB_SIZE, GIB_SIZE), color, BOX_GIB, BOX_STATIC);

    if(isFloating)
    {
        gib->activateFloatGibs();
    }

    gibs.push_back(gib);
}

void World::createGibBunch(b2World& world, int amount, int x, int y, sf::Color color, bool isFloating)
{
    for(int i = 0; i < amount; i++)
    {
        createGib(world, x, y, color, isFloating);
    }
}

std::string World::getRandomBoxType(bool isEnemy)
{
    std::string type = "box";

    if(isEnemy)
    {
        if(rand() % 1000 < WORLD_CHANCE_BOOMBOX_ENEMY)
        {
            type = "boombox";
        }
        else if(rand() % 1000 < WORLD_CHANCE_FLOATBOX_ENEMY)
        {
            type = "floatbox";
        }
        else if(rand() % 1000 < WORLD_CHANCE_MAGICBOX_ENEMY)
        {
            type = "magicbox";
        }
    }
    else
    {
        if(rand() % 1000 < WORLD_CHANCE_BOOMBOX)
        {
            type = "boombox";
        }
        else if(rand() % 1000 < WORLD_CHANCE_CHARGEBOX)
        {
            type = "chargebox";
        }
        else if(rand() % 1000 < WORLD_CHANCE_FLOATBOX)
        {
            type = "floatbox";
        }
        else if(rand() % 1000 < WORLD_CHANCE_MAGICBOX)
        {
            type = "magicbox";
        }
    }

    return type;
}

short int World::getRandomTeam(std::string type)
{

    if(type == "boombox" && rand() % 1000 < WORLD_CHANCE_BOOMBOX_ENEMY)
    {
        return TEAM_ENEMY;
    }

    if(type == "floatbox" && rand() % 1000 < WORLD_CHANCE_FLOATBOX_ENEMY)
    {
        return TEAM_ENEMY;
    }

    if(type == "magicbox" && rand() % 1000 < WORLD_CHANCE_MAGICBOX_ENEMY)
    {
        return TEAM_ENEMY;
    }

    return TEAM_NEUTRAL;
}

void World::generateWorld(b2World& physicsWorld)
{
    srand(time(0));

    // ------------------------------- Fill world with flat surface -------------------------------
    for(int i = 0; i < MAX_WORLD_HEIGHT; i++)
    {
        for(int j = 0; j < MAX_WORLD_WIDTH; j++)
        {
            createGroundBox(physicsWorld, j*BOX_SIZE, GROUND_STARTING_HEIGHT + i*BOX_SIZE, 30.0f, 0.35f, getRandomBoxType(false));
        }
    }

    int rndMod = 1;

    // ------------------------------- Randomly kill boxes up to MAX_WORLD_HEIGHT / rndMod rows -------------------------------
    for(int i = 0; i < MAX_WORLD_HEIGHT; i++)
    {
        for(int j = 0; j < MAX_WORLD_WIDTH; j++)
        {
            std::stringstream index;
            int x = j*BOX_SIZE;
            int y = GROUND_STARTING_HEIGHT + i*BOX_SIZE;
            index << x << "x" << y;

            int rnd = rand() % 100;

            if((i+1)*rndMod < rnd)
            {
                deleteGroundBoxById(index.str());
            }
        }
    }

    // ------------------------------- Fill holes & delete loners -------------------------------
    for(int i = 0; i < MAX_WORLD_HEIGHT; i++)
    {
        for(int j = 0; j < MAX_WORLD_WIDTH; j++)
        {
            int x = j*BOX_SIZE;
            int y = GROUND_STARTING_HEIGHT + i*BOX_SIZE;

            neighbour neighbours[9];
            int center = 4;
            int id = 0;
            int empty = 0;
            int fillThreshold = 6;
            int killThreshold = 5;

            // Find neighbours
            for(int l = -1; l < 2; l++)
            {
                for(int m = -1; m < 2; m++)
                {
                    neighbours[id].x = x + m*BOX_SIZE;
                    neighbours[id].y = y + l*BOX_SIZE;

                    std::stringstream index;
                    index << neighbours[id].x << "x" << neighbours[id].y;

                    neighbours[id].index = index.str();

                    neighbours[id].exists = groundBoxExists(neighbours[id].index) ? true : false;

                    if(id != center && !neighbours[id].exists)
                    {
                        empty++;
                    }

                    id++;
                }
            }

            // Fill & kill
            if(!neighbours[center].exists && empty <= fillThreshold)
            {
                createGroundBox(physicsWorld, neighbours[center].x, neighbours[center].y, 30.0f, 0.35f, getRandomBoxType(false));
            }
            else if(neighbours[center].exists && empty >= killThreshold)
            {
                for(int k = 0; k < 9; k++)
                {
                    if(neighbours[k].exists)
                    {
                        deleteGroundBoxById(neighbours[k].index);
                    }
                }
            }
        }
    }

    // ------------------------------- Create canyon -------------------------------
    if(rand() % 1000 < WORLD_CHANCE_CANYON)
    {
        int startingX = rand() % (int)(MAX_WORLD_WIDTH - 50) + 50;
        //int startingX = 20;
        int length = rand() % 3 + 4;
        int prevLength = 1;
        bool toggle = false;

        for(int i = 0; i < MAX_WORLD_HEIGHT; i++)
        {
            int randomLength = prevLength + length + rand() % 3;

            for(int j = startingX - randomLength; j < startingX + randomLength; j++)
            {
                std::stringstream index;
                int x = j*BOX_SIZE;
                int y = GROUND_STARTING_HEIGHT + i*BOX_SIZE;
                index << x << "x" << y;

                if(groundBoxExists(index.str()))
                {
                    deleteGroundBoxById(index.str());
                }
            }


            if(prevLength < length+1 && !toggle)
            {
                prevLength++;
            }
            else
            {
                toggle = true;
                prevLength--;

                if(prevLength == 1)
                {
                    toggle = false;
                }
            }
        }
    }

    // ------------------------------- Create flying islands -------------------------------
//    int amountIslands = rand() % WORLD_VARY_AMOUNT_FLYING_ISLANDS + WORLD_AMOUNT_FLYING_ISLANDS;
//    //int amountIslands = 1;
//
//    for(int h = 0; h < amountIslands; h++)
//    {
//        int startX = rand() % MAX_WORLD_WIDTH * BOX_SIZE - GROUND_STARTING_HEIGHT;
//        int startY = rand() % 1000 - 1000;
//
////        int startX = 20;
////        int startY = 0;
//        int size = rand() % 7 + 7;
//
//        b2Vec2 start = getGroundCoordsByCoordinates(startX, startY);
//        std::vector<neighbour> indices;
//
//        for(int i = 1; i < size; i++)
//        {
//            for(int j = 1; j < size * 3; j++)
//            {
//                createGroundBox(physicsWorld, start.x + j * BOX_SIZE , start.y + -i * BOX_SIZE, 30.0f, 0.35f, getRandomBoxType());
//                std::stringstream index;
//                index << start.x + j * BOX_SIZE << "x" << start.y + -i * BOX_SIZE;
//
//                neighbour n;
//                n.x = start.x + j * BOX_SIZE;
//                n.y = start.y + -i * BOX_SIZE;
//                n.exists = true;
//                n.index = index.str();
//
//                indices.push_back(n);
//            }
//        }
//
//        for(unsigned int i = 0; i < indices.size(); i++)
//        {
//            // Top
//            std::stringstream index;
//            index << indices.at(i).x << "x" << indices.at(i).y - BOX_SIZE;
//
//            if(!groundBoxExists(index.str()) && rand() % 100 < 50)
//            {
//                createGroundBox(physicsWorld, indices.at(i).x, indices.at(i).y - BOX_SIZE, 30.0f, 0.35f, getRandomBoxType());
//            }
//
//            // Bottom
//            index.str("");
//            index.clear();
//            index << indices.at(i).x << "x" << indices.at(i).y + BOX_SIZE;
//
//            if(!groundBoxExists(index.str()) && rand() % 100 < 50)
//            {
//                createGroundBox(physicsWorld, indices.at(i).x, indices.at(i).y + BOX_SIZE, 30.0f, 0.35f, getRandomBoxType());
//            }
//
//            // Left
//            index.str("");
//            index.clear();
//            index << indices.at(i).x - BOX_SIZE << "x" << indices.at(i).y;
//
//            if(!groundBoxExists(index.str()) && rand() % 100 < 50)
//            {
//                createGroundBox(physicsWorld, indices.at(i).x - BOX_SIZE, indices.at(i).y, 30.0f, 0.35f, getRandomBoxType());
//            }
//
//            // Left
//            index.str("");
//            index.clear();
//            index << indices.at(i).x + BOX_SIZE << "x" << indices.at(i).y;
//
//            if(!groundBoxExists(index.str()) && rand() % 100 < 50)
//            {
//                createGroundBox(physicsWorld, indices.at(i).x + BOX_SIZE, indices.at(i).y, 30.0f, 0.35f, getRandomBoxType());
//            }
//        }
//    }

    // ------------------------------- Set up neighbour relations -------------------------------
    for(std::map<std::string, Box*>::iterator it = groundBoxes.begin(); it != groundBoxes.end(); ++it)
    {
        // Get grid coordinates
        int x = it->second->getBody()->GetPosition().x * SCALE;
        int y = it->second->getBody()->GetPosition().y * SCALE;

        b2Vec2 pos = getGroundCoordsByCoordinates(x, y);

        x = pos.x;
        y = pos.y;

        int center = 4;
        int id = 0;
        int empty = 0;

        // Find neighbours
        for(int l = -1; l < 2; l++)
        {
            for(int m = -1; m < 2; m++)
            {
                it->second->neighbours[id].x = x + m*BOX_SIZE;
                it->second->neighbours[id].y = y + l*BOX_SIZE;

                std::stringstream index;
                index << it->second->neighbours[id].x << "x" << it->second->neighbours[id].y;

                it->second->neighbours[id].index = index.str();

                it->second->neighbours[id].exists = groundBoxExists(it->second->neighbours[id].index) ? true : false;

                if(id != center && !it->second->neighbours[id].exists)
                {
                    empty++;
                }

                id++;
            }
        }

        if(empty > 0)
        {
            it->second->setBodyActive(true);
        }
    }

    // ------------------------------- Create outer boundaries -------------------------------
    //createWorldBoundaries(physicsWorld);


    // ------------------------------- Ally team -------------------------------
    int startX = rand() % 200 + 400;
    int startY = rand() % 200 + 200;
    int amountNormalBoxes = 5;
    int amountChargeBoxes = 3;
    int amountMagicBoxes = 2;

    // Boxes
    for(int i = 0; i < amountNormalBoxes; i++)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "box");
    }

    // Charge Boxes
    for(int i = 0; i < amountChargeBoxes; i++)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "chargebox");
    }

    // Magic Boxes
    for(int i = 0; i < amountMagicBoxes; i++)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "magicbox");
    }

    // Random chance boxes
    if(rand() % 100 < 20)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "chargebox");
    }

//    if(rand() % 100 < 20)
//    {
//        startX = rand() % 200 + 400;
//        startY = rand() % 200;
//        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "floatbox");
//    }

    if(rand() % 100 < 20)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "magicbox");
    }

    if(rand() % 100 < 20)
    {
        startX = rand() % 200 + 400;
        startY = rand() % 200 + 200;
        createBox(physicsWorld, startX, startY, 30.0f, 0.35f, TEAM_ALLY, "boombox");
    }

    // ------------------------------- Enemies scattered in the world -------------------------------
    int eStartX = rand() % (int)(MAX_WORLD_WIDTH * BOX_SIZE - 1500) + 1500;
    int eStartY = rand() % 200;

    for(int i = 0; i < 60; i++)
    {
        createBox(physicsWorld, eStartX, eStartY, 30.0f, 0.35f, TEAM_ENEMY, getRandomBoxType(true));
        //createBox(physicsWorld, eStartX, eStartY, 30.0f, 0.35f, TEAM_ENEMY, "floatbox");

        eStartX = rand() % (int)(MAX_WORLD_WIDTH * BOX_SIZE - 1500) + 1500;
        eStartY = rand() % 200;
    }

    // FoxBox
    createBox(physicsWorld, MAX_WORLD_WIDTH * BOX_SIZE - BOX_SIZE, 320, 30.0f, 0.35f, TEAM_NEUTRAL, "foxbox");
}

void World::regenerateWorld(b2World& physicsWorld)
{
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        boxes.at(i)->getBody()->GetWorld()->DestroyBody(boxes.at(i)->getBody());
    }

    boxes.clear();

    // Iterate through all static boxes
    for(std::map<std::string, Box*>::iterator it = groundBoxes.begin(); it != groundBoxes.end(); ++it)
    {
        it->second->getBody()->GetWorld()->DestroyBody(it->second->getBody());
    }

    groundBoxes.clear();
}

void World::createWorldBoundaries(b2World& world)
{
    float sideWallLeft = -BOX_SIZE / SCALE;
    float sideWallRight = (BOX_SIZE * MAX_WORLD_WIDTH) / SCALE;
    float sideWallCenterY = (((MAX_SKY_HEIGHT + GROUND_STARTING_HEIGHT + MAX_WORLD_HEIGHT * BOX_SIZE) / 2) - (MAX_SKY_HEIGHT + GROUND_STARTING_HEIGHT)) / SCALE;

    float sideWallHeight = ((MAX_SKY_HEIGHT + GROUND_STARTING_HEIGHT + MAX_WORLD_HEIGHT * BOX_SIZE) / 2) / SCALE;
    float sideWallWidth = BOX_SIZE/2.0f / SCALE;

    // Left wall
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(sideWallLeft, sideWallCenterY);
    bodyDef.type = b2_staticBody;
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(sideWallWidth, sideWallHeight);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.7f;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    // Right wall
    bodyDef.position = b2Vec2(sideWallRight, sideWallCenterY);
    bodyDef.type = b2_staticBody;
    body = world.CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
}

std::vector<Box*>* World::getBoxes()
{
    return &boxes;
}

std::map<std::string, Box*>* World::getGroundBoxes()
{
    return &groundBoxes;
}

sf::Texture& World::getTexture(std::string texName)
{
    return textures.at(texName);
}

void World::update(Events gameEvents, b2World& world, Player& player)
{
    // Iterate through all static boxes
    for(std::map<std::string, Box*>::iterator it = groundBoxes.begin(); it != groundBoxes.end(); ++it)
    {
        int boxX = it->second->getBody()->GetPosition().x * SCALE;
        int boxY = it->second->getBody()->GetPosition().y * SCALE;

        // Update static boxes ------------------------------- POSSIBLE OPTIMIZATION: Update inactive boxes only if within view range; Check with game logic -------------------------------
        it->second->update(gameEvents);

        // Check for conversion
        if(it->second->isFlaggedForConversion)
        {
            boxes.push_back(it->second);
            boxes.at(boxes.size() - 1)->getBody()->SetType(b2_dynamicBody);
            boxes.at(boxes.size() - 1)->isConverted = true;

            it->second->notifyNeighbours();
            it->second->notifyContactsOnConversion(true);
            groundBoxes.erase(it);

            continue;
        }

        // We need to check for dead boxes outside of a box itself
        if(it->second->isDead)
        {
            it->second->notifyNeighbours();

            if(it->second->lastDamageReceivedFrom == TEAM_ALLY)
            {
                if(it->second->type == "box")
                {
                    player.addResources(BOX_VALUE);
                }
                else if(it->second->type == "chargebox")
                {
                    player.addResources(CHARGEBOX_VALUE);
                }
                else if(it->second->type == "magicbox")
                {
                    player.addResources(MAGICBOX_VALUE);
                }
                else if(it->second->type == "floatbox")
                {
                    player.addResources(FLOATBOX_VALUE);
                }
                else if(it->second->type == "boombox")
                {
                    player.addResources(BOOMBOX_VALUE);
                }
            }

            bool isFloating = (it->second->type == "floatbox") ? true : false;

            if(it->second->team == TEAM_NEUTRAL)
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::White, isFloating);
            }
            else if(it->second->team == TEAM_ALLY)
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::Green, isFloating);
            }
            else
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::Red, isFloating);
            }

            box_dead.play();

            b2Vec2 pos = getGroundCoordsByCoordinates(boxX, boxY);
            deleteGroundBoxById(getIndexByGroundCoordinates(pos.x, pos.y));
            continue;
        }
    }

    // Iterate through all dynamic boxes
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        int boxX = boxes.at(i)->getBody()->GetPosition().x * SCALE;
        int boxY = boxes.at(i)->getBody()->GetPosition().y * SCALE;

        if(boxes.at(i)->getBody()->GetFixtureList()->GetFilterData().categoryBits == BOX_STATIC && gibs.size() == 0)
        {
            b2Filter filter;
            for(b2Fixture* f = boxes.at(i)->getBody()->GetFixtureList(); f; f = f->GetNext())
            {
                filter = f->GetFilterData();
                filter.categoryBits = BOX_DYNAMIC;
                filter.maskBits = BOX_STATIC | BOX_DYNAMIC;
                f->SetFilterData(filter);
            }
        }

        // Update dynamic boxes
        boxes.at(i)->update(gameEvents);

        // We need to check for dead boxes outside of a box itself
        if(boxes.at(i)->isDead)
        {
            // Notify any box that has a connection to this one
            boxes.at(i)->notifyDynamics();

            bool isFloating = (boxes.at(i)->type == "floatbox") ? true : false;

            if(boxes.at(i)->team == TEAM_NEUTRAL)
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::White, isFloating);
            }
            else if(boxes.at(i)->team == TEAM_ALLY && boxes.at(i)->type != "magicbox")
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::Green, isFloating);
            }
            else if(boxes.at(i)->team == TEAM_ENEMY && boxes.at(i)->type != "magicbox")
            {
                createGibBunch(world, GIB_AMOUNT, boxX, boxY, sf::Color::Red, isFloating);
            }

            if(boxes.at(i)->lastDamageReceivedFrom == TEAM_ALLY)
            {
                if(boxes.at(i)->type == "box")
                {
                    player.addResources(BOX_VALUE);
                }
                else if(boxes.at(i)->type == "chargebox")
                {
                    player.addResources(CHARGEBOX_VALUE);
                }
                else if(boxes.at(i)->type == "magicbox")
                {
                    player.addResources(MAGICBOX_VALUE);
                }
                else if(boxes.at(i)->type == "floatbox")
                {
                    player.addResources(FLOATBOX_VALUE);
                }
                else if(boxes.at(i)->type == "boombox")
                {
                    player.addResources(BOOMBOX_VALUE);
                }
            }

            box_dead.play();

            deleteBoxById(i);
            player.selectedBoxes.clear();
            player.selectedBoxes = getSelectedBoxes();
            continue;
        }

        if(resourceTimer.timeReached() && boxes.at(i)->team == TEAM_ALLY)
        {
            if(boxes.at(i)->type == "box")
            {
                player.addResources(BOX_VALUE);
            }
            else if(boxes.at(i)->type == "chargebox")
            {
                player.addResources(CHARGEBOX_VALUE);
            }
            else if(boxes.at(i)->type == "magicbox")
            {
                player.addResources(MAGICBOX_VALUE);
            }
            else if(boxes.at(i)->type == "floatbox")
            {
                player.addResources(FLOATBOX_VALUE);
            }
            else if(boxes.at(i)->type == "boombox")
            {
                player.addResources(BOOMBOX_VALUE);
            }
        }
    }

    for(unsigned int i = 0; i < gibs.size(); i++)
    {
        gibs.at(i)->update();

        if(gibs.at(i)->isDead)
        {
            deleteGibById(i);
            continue;
        }
    }

    if(resourceTimer.timeReached())
    {
        resourceTimer.resetTimer();
    }

    resourceTimer.update();

    if(groundBoxes.size() == 0 && boxes.size() == 0)
    {
        generateWorld(world);
    }

    // Music
    if(music.getStatus() == sf::Sound::Stopped)
    {
        music.openFromFile(musicStrings.at(rand() % 5));
        music.play();
    }

    if(global_levelComplete)
    {
        regenerateWorld(world);
    }
}

void World::draw(sf::RenderWindow& window, b2Vec2 winTopLeft, b2Vec2 winBottomRight, bool isMinimap)
{
    // Iterate through all static boxes
    for(std::map<std::string, Box*>::iterator it = groundBoxes.begin(); it != groundBoxes.end(); ++it)
    {
        int boxX = it->second->getBody()->GetPosition().x * SCALE;
        int boxY = it->second->getBody()->GetPosition().y * SCALE;

        // Render only if within viewable area
        if(boxX <= winBottomRight.x && boxX >= winTopLeft.x && boxY <= winBottomRight.y && boxY >= winTopLeft.y)
        {
            // Draw box
            it->second->draw(window);
        }
    }

    // Iterate through all dynamic boxes
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        int boxX = boxes.at(i)->getBody()->GetPosition().x * SCALE;
        int boxY = boxes.at(i)->getBody()->GetPosition().y * SCALE;

        // Render only if within viewable area
        if(boxX <= winBottomRight.x && boxX >= winTopLeft.x && boxY <= winBottomRight.y && boxY >= winTopLeft.y)
        {
            // Draw box
            if(!isMinimap)
            {
                boxes.at(i)->draw(window);
            }
            else
            {
                if(boxes.at(i)->team == TEAM_ALLY)
                {
                    boxes.at(i)->draw(window);
                }
            }
        }
    }

    for(unsigned int i = 0; i < gibs.size(); i++)
    {
        gibs.at(i)->draw(window);
    }

    // Iterate through all dynamic boxes (HUD LOOP)
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        int boxX = boxes.at(i)->getBody()->GetPosition().x * SCALE;
        int boxY = boxes.at(i)->getBody()->GetPosition().y * SCALE;

        // Render only if within viewable area
        if(boxX <= winBottomRight.x && boxX >= winTopLeft.x && boxY <= winBottomRight.y && boxY >= winTopLeft.y)
        {
            // Draw box
            if(!isMinimap)
            {
                boxes.at(i)->displayAdditionals(window);
            }
            else
            {
                if(boxes.at(i)->team == TEAM_ALLY)
                {
                    boxes.at(i)->displayAdditionals(window);
                }
            }
        }
    }
}

int World::getBoxIdByCoordinates(int x, int y)
{
    x += BOX_SIZE/2;
    y += BOX_SIZE/2;

    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        int minX = boxes.at(i)->getBody()->GetPosition().x * SCALE;
        int maxX = minX + BOX_SIZE;
        int minY = boxes.at(i)->getBody()->GetPosition().y * SCALE;
        int maxY = minY + BOX_SIZE;

        if(x <= maxX && x >= minX && y <= maxY && y >= minY)
        {
            return i;
        }
    }

    return -1;
}

Box* World::getBoxById(int id)
{
    return boxes.at(id);
}

void World::deleteBoxById(int id)
{
    boxes.at(id)->getBody()->GetWorld()->DestroyBody(boxes.at(id)->getBody());
    boxes.erase(boxes.begin() + id);
}

void World::deleteGibById(int id)
{
    gibs.at(id)->getBody()->GetWorld()->DestroyBody(gibs.at(id)->getBody());
    gibs.erase(gibs.begin() + id);
}

void World::deleteGroundBoxById(std::string id)
{
    groundBoxes.at(id)->getBody()->GetWorld()->DestroyBody(groundBoxes.at(id)->getBody());
    groundBoxes.erase(id);
}

void World::toggleBoxSelect(int id)
{
    if(!boxes.at(id)->isActive)
    {
        boxes.at(id)->isActive = true;
    }
    else
    {
        boxes.at(id)->isActive = false;
    }

    boxes.at(id)->updateTexture();
}

std::vector<int> World::selectAllDynamicBoxes()
{
    std::vector<int> ids;

    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            boxes.at(i)->isActive = true;
            boxes.at(i)->updateTexture();
            ids.push_back(i);
        }
    }

    return ids;
}

void World::changeBoxTeam(int id, int team)
{
    boxes.at(id)->team = team;
}

std::vector<int> World::getBoxesBySelection(sf::Vector2f position, sf::Vector2f size)
{
    std::vector<int> ids;
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            int centerX = boxes.at(i)->getBody()->GetPosition().x * SCALE;
            int centerY = boxes.at(i)->getBody()->GetPosition().y * SCALE;

            if(centerX <= position.x + size.x && centerX >= position.x && centerY <= position.y + size.y && centerY >= position.y)
            {
                if(!boxes.at(i)->isActive)
                {
                    toggleBoxSelect(i);
                    ids.push_back(i);
                }
            }
        }
    }

    return ids;
}

std::vector<int> World::getSelectedBoxes()
{
    std::vector<int> ids;
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->getBody()->GetType() == b2_dynamicBody && boxes.at(i)->team == TEAM_ALLY && boxes.at(i)->isActive)
        {
            ids.push_back(i);
        }
    }

    return ids;
}

void World::setBoxTargetArea(int id, TargetArea target)
{
    boxes.at(id)->hasActiveTargetArea = true;
    boxes.at(id)->target = target;
}

void World::setBunchTargetArea(std::vector<int> ids, TargetArea target)
{
    for(unsigned int i = 0; i < ids.size(); i++)
    {
        setBoxTargetArea(ids.at(i), target);
    }
}

int World::getAllyBoxAmount()
{
    int amount = 0;
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            amount++;
        }
    }

    return amount;
}

int World::getAllBoxChargeAmount()
{
    int charges = 0;
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            charges += boxes.at(i)->tagCharges;
        }
    }

    return charges;
}

std::vector<int> World::getAllAlliesByType(std::string type)
{
    std::vector<int> ids;

    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            if(boxes.at(i)->type == type)
            {
                if(!boxes.at(i)->isActive)
                {
                    toggleBoxSelect(i);
                }

                ids.push_back(i);
            }
            else
            {
                if(boxes.at(i)->isActive)
                {
                    toggleBoxSelect(i);
                }
            }
        }
    }

    return ids;
}

int World::getChargedBox()
{
    deselectAllBoxes();

    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            if(boxes.at(i)->tagCharges > 0)
            {
                if(!boxes.at(i)->isActive)
                {
                    toggleBoxSelect(i);
                }

                return i;
            }
        }
    }

    return -1;
}

void World::deselectAllBoxes()
{
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            boxes.at(i)->isActive = false;
            boxes.at(i)->updateTexture();
        }
    }
}

void World::stopAllActions()
{
    for(unsigned int i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i)->team == TEAM_ALLY)
        {
            boxes.at(i)->stop();
        }
    }
}
