#include "UI.h"

UI::UI()
{

}

UI::~UI()
{
    //dtor
}

void UI::init()
{
    id_boxAmount = addText("", 0.025f, 0.05f, DEFAULT_TEXT_SIZE, sf::Color::Black);
    id_boxChargeAmount = addText("", 0.025f, 0.1f, DEFAULT_TEXT_SIZE, sf::Color::Black);
    id_resources = addText("", 0.025f, 0.15f, DEFAULT_TEXT_SIZE, sf::Color::Black);
    id_ai = addText("", 0.025f, 0.2f, DEFAULT_TEXT_SIZE, sf::Color::Black);
    id_globalMessage = addText("", 0.33f, 0.6f, 30, sf::Color::Black);

    globalMessageTimer.setThreshold(4.0f);
    globalMessageTimer.endTimer();
}

void UI::setSize(int width, int height)
{
    this->width = width;
    this->height = height;

    hudView.setSize(width, height);

    setCenter();
}

void UI::setCenter()
{
    hudView.setCenter(width/2.0f, height/2.0f);
}

sf::View UI::getView()
{
    return hudView;
}

void UI::update(World gameWorld, Player player)
{
    std::stringstream amount;
    amount << "Total Friendly Boxes: " << gameWorld.getAllyBoxAmount();
    std::stringstream chargeAmount;
    chargeAmount << "Total Charges left: " << gameWorld.getAllBoxChargeAmount();
    std::stringstream resourceAmount;
    resourceAmount << "Resources: " << player.getResources();
    std::string aiStatus = "";

    if(global_enableAllyAI)
    {
        aiStatus = "AI: ON";
    }
    else
    {
        aiStatus = "AI: OFF";
    }

    texts.at(id_boxAmount).setString(amount.str());
    texts.at(id_boxChargeAmount).setString(chargeAmount.str());
    texts.at(id_resources).setString(resourceAmount.str());
    texts.at(id_ai).setString(aiStatus);

    globalMessageTimer.update();
}

void UI::draw(sf::RenderWindow& window)
{
    for(unsigned int i = 0; i < texts.size(); i++)
    {
        if(i != id_globalMessage)
        {
            window.draw(texts.at(i));
        }
        else
        {
            if(!globalMessageTimer.timeReached())
            {
                window.draw(texts.at(i));
            }
        }
    }

    if(global_isPaused)
    {
        sf::Text text_pause("Paused", font_default, 100);
        text_pause.setColor(sf::Color::White);
        text_pause.setPosition(window_width * 0.38f, window_height * 0.43f);

        window.draw(text_pause);
    }
}

int UI::addText(std::string text, float x, float y, unsigned int size, sf::Color color)
{
    sf::Text temp("", font_default, DEFAULT_TEXT_SIZE);
    temp.setString(text);

    float pixX = (float)window_width * x;
    float pixY = (float)window_height * y;

    temp.setPosition(pixX, pixY);
    temp.setCharacterSize(size);
    temp.setColor(color);
    texts.push_back(temp);

    return texts.size()-1;
}

void UI::setGlobalMessage(std::string msg)
{
    texts.at(id_globalMessage).setString(msg);
    globalMessageTimer.resetTimer();
}
