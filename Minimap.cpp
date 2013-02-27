#include "Minimap.h"

Minimap::Minimap()
{
    newViewCenter = sf::Vector2f(0.0f, 0.0f);
    updateViewThroughMinimap = false;
    outLineThickness = 30.0f;
}

Minimap::~Minimap()
{
    //dtor
}

void Minimap::setSize(int width, int height, int wWidth, int wHeight)
{
    windowWidth = (float)wWidth;
    windowHeight = (float)wHeight;

    scale = (float)windowWidth / windowHeight;

    this->width = width;
    this->height = height/scale;

    calcCenter();
}

void Minimap::calcCenter()
{
    widthMod = BOX_SIZE/2.0f;
    heightMod = (height - MAX_WORLD_HEIGHT * BOX_SIZE) - GROUND_STARTING_HEIGHT;
    centerX = width / 2.0f - widthMod;
    centerY = height / 2.0f - heightMod;

    background.setPosition((float)centerX - this->width/2, (float)centerY - this->height/2);
    background.setSize(sf::Vector2f(this->width, this->height));
    background.setFillColor(sf::Color::Black);
}

void Minimap::calcViewport(float size)
{
    float borderX = (MINIMAP_BORDER_SIZE / 2.0f) / windowWidth;
    float borderY = (MINIMAP_BORDER_SIZE / 2.0f) / windowHeight;

    viewportX = 0.0 + borderX;
    viewportY = 1.0 - size - borderY;
    viewportWidth = size;
    viewportHeight = size;
}

sf::View Minimap::getUpdatedView()
{
    minimap.setSize(width, height);
    minimap.setCenter(centerX, centerY);
    sf::FloatRect viewport(viewportX, viewportY, viewportWidth, viewportHeight);
    minimap.setViewport(viewport);

    return minimap;
}

void Minimap::processClicks(Events& gameEvents)
{
    if(gameEvents.mouseLeftClicked)
    {
        float percX = gameEvents.lastMouseWindow.x / windowWidth;
        float percY = gameEvents.lastMouseWindow.y / windowHeight - (1.0f - viewportHeight);

        float truePercX = percX / viewportWidth;
        float truePercY = percY / viewportHeight;

        newViewCenter.x = width * truePercX - widthMod;
        newViewCenter.y = height * truePercY - heightMod;

        std::cout << newViewCenter.y << std::endl;

        updateViewThroughMinimap = true;

        gameEvents.mouseLeftHeld = false;
    }
    else
    {
        updateViewThroughMinimap = false;
    }
}

void Minimap::setCameraPosition(int x, int y)
{
    camera.setPosition(x - windowWidth / 2, y - windowHeight / 2);
    camera.setSize(sf::Vector2f(windowWidth, windowHeight));
    camera.setOutlineColor(sf::Color::Green);
    camera.setOutlineThickness(outLineThickness);
    camera.setFillColor(sf::Color::Transparent);

}

void Minimap::update(Events& gameEvents)
{
    float percX = gameEvents.lastMouseWindow.x / windowWidth;
    float percY = gameEvents.lastMouseWindow.y / windowHeight;

    if(percX >= 0.0f && percX <= viewportWidth)
    {
        if(percY >= viewportY && percY <= 1.0f)
        {
            processClicks(gameEvents);
        }
    }
}

void Minimap::draw(sf::RenderWindow& window)
{
    window.draw(background);
}

void Minimap::drawCameraWindow(sf::RenderWindow& window)
{
    window.draw(camera);
}
