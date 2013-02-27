#include "Gibs.h"

Gibs::Gibs()
{
    isDead = false;
    fadeTimer.setThreshold(3.0f);
}

Gibs::~Gibs()
{
    //dtor
}

void Gibs::createGib(b2World& physicsWorld, b2Vec2 position, b2Vec2 size, sf::Color color, uint16 categoryBits, uint16 maskBits)
{
    // Physics body initialization
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(position.x/SCALE, position.y/SCALE);
    bodyDef.type = b2_dynamicBody;
    body = physicsWorld.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((size.x/2)/SCALE, (size.y/2)/SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 4.0f;
    fixtureDef.friction = 0.05f;
    fixtureDef.restitution = 0.05f;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;

    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    // Rect initilization
    rect.setOrigin(size.x/2.0f, size.y/2.0f);
    rect.setPosition(position.x, position.y);
    rect.setSize(sf::Vector2f(size.x, size.y));
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(color);
    rect.setOutlineThickness(1.0f);

    this->color = color;

    setRandomForceToCenter();
}

void Gibs::setRandomForceToCenter()
{
    b2Vec2 force((rand() % 250 - 125), (rand() % 250 - 125));

    body->ApplyForceToCenter(force);

    if(force.x < 0)
    {
        body->SetAngularVelocity(-5.0f);
    }
    else
    {
        body->SetAngularVelocity(5.0f);
    }
}

void Gibs::update()
{
    rect.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
    rect.setRotation(180/b2_pi * body->GetAngle());

    float percTime = fadeTimer.getCurrentTime() / fadeTimer.getThreshold();
    float fadeValue = 255 * percTime;

    color = sf::Color(color.r, color.g, color.b, 255 - fadeValue);

    rect.setFillColor(sf::Color(255, 255, 255, 255 - fadeValue));
    rect.setOutlineColor(color);

    fadeTimer.update();

    if(fadeTimer.timeReached())
    {
        isDead = true;
    }
}

void Gibs::draw(sf::RenderWindow& window)
{
    window.draw(rect);
}

b2Body* Gibs::getBody()
{
    return body;
}

void Gibs::activateFloatGibs()
{
    body->SetGravityScale(0.0f);
    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -5.0f));
}
