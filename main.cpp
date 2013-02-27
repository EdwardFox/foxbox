#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "utility.h"
#include "Box.h"
#include "BoomBox.h"
#include "Events.h"
#include "World.h"
#include "Player.h"
#include "BoxContactListener.h"
#include "Minimap.h"
#include "UI.h"
#include "Menu.h"
#include "MenuImage.h"
#include "MenuButton.h"

// Variables
int antiAliasLevel = 8;
std::string name = "Foxbox";

b2Vec2 gravity(0.0f, 9.8f);
b2World physicsWorld(gravity, true);
World gameWorld;
Events gameEvents;
Player player;
UI ui;

sf::ContextSettings settings(0, 0, antiAliasLevel, 2, 0);
sf::RenderWindow window(sf::VideoMode(window_width, window_height), name, sf::Style::Default, settings);

// Menu
Menu menu;
MenuImage* menu_logo;
MenuImage* menu_logo_bottom;
MenuButton* menu_button_play;
MenuButton* menu_button_resume;
MenuButton* menu_button_quit;
MenuButton* menu_button_instructions;

MenuButton* menu_button_instructions_controls;
MenuButton* menu_button_instructions_gameplay;

bool show_instructions = false;
bool show_instructions_controls = true;
bool show_instructions_gameplay = false;


// Functions
sf::View updateView(sf::View view);
void initializeMenu();
void play();
void resume();
void instructions();
void quit();

void instructions_controls();
void instructions_gameplay();

void showInstructions();
void showControls();
void showGameplay();

int main()
{
    // Initialize globals
    initGlobals();

    // Window
    window.setSize(sf::Vector2u(window_width, window_height));
    window.setPosition(sf::Vector2i(200, 200));
    window.setFramerateLimit(FRAMES_PER_SECOND);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // Camera view
    sf::View windowView;

    // Menu
    initializeMenu();

    // UI
    ui.init();

    // Minimap
//    Minimap minimap;

    // Create & Set contactlistener
    BoxContactListener boxContactListener;
    physicsWorld.SetContactListener(&boxContactListener);

    // Build world
    //gameWorld.generateWorld(physicsWorld);

    // ------------------------------- MAIN LOOP -------------------------------
    while(window.isOpen())
    {
        // ------------------------------- Input & Views -------------------------------
        sf::Event event;
        gameEvents.processEvents(window, event);

        // Update view in case of window resize
        window_width = window.getSize().x;
        window_height = window.getSize().y;
        windowView.setSize(window_width, window_height);

//        if(minimap.updateViewThroughMinimap)
//        {
//            windowView.setCenter(minimap.newViewCenter.x, minimap.newViewCenter.y);
//        }

        if(player.hasNewFocus)
        {
            windowView.setCenter(player.getNewFocus().x, player.getNewFocus().y);
        }

        // Update normal view with inputs
        windowView = updateView(windowView);

        if(global_levelComplete)
        {
            windowView.setCenter(window_width/2, window_height/2);
            global_levelComplete = false;
        }

        // Clear window
        window.clear(sf::Color(255, 255, 255, 255));

        if(global_isMenu)
        {
            ui.setSize(window_width, window_height);
            window.setView(ui.getView());

            menu_logo_bottom->setPosition(window_width - textures["menu_logo_bottom"].getSize().x, window_height - textures["menu_logo_bottom"].getSize().y);

            menu.update(gameEvents);
            menu.draw(window);

            // Instructions
            if(show_instructions)
            {
                showInstructions();

                if(show_instructions_controls)
                {
                    showControls();
                }

                if(show_instructions_gameplay)
                {
                    showGameplay();
                }
            }
        }
        else
        {
            window.setView(windowView);

            // ------------------------------- Updates -------------------------------

            // Player
            player.update(physicsWorld, gameEvents, gameWorld);

            if(!global_isPaused)
            {
                // Physics
                physicsWorld.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

                // World
                gameWorld.update(gameEvents, physicsWorld, player);

                // UI
                ui.update(gameWorld, player);
            }


            // Calculate viewable area
            int viewShiftX = window.getSize().x/2 - window.getView().getCenter().x;
            int viewShiftY = window.getSize().y/2 - window.getView().getCenter().y;
            int windowMinX = -100 - viewShiftX;
            int windowMaxX = windowMinX + window_width + 200;
            int windowMinY = -100 - viewShiftY;
            int windowMaxY = windowMinY + window_height + 200;

            // ------------------------------- Drawing -------------------------------

            window.setView(ui.getView());
            // Background
            sf::Vertex rectangle[] =
            {
                sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Color(0, 100, 130, 255)),
                sf::Vertex(sf::Vector2f(window_width, 0.0f), sf::Color(0, 100,130, 255)),
                sf::Vertex(sf::Vector2f(window_width, window_height), sf::Color(0, 200, 230, 255)),
                sf::Vertex(sf::Vector2f(0.0f, window_height), sf::Color(0, 200, 230, 255))
            };
            window.draw(rectangle, 4, sf::Quads);

            window.setView(windowView);

            // World & Player
            gameWorld.draw(window, b2Vec2(windowMinX, windowMinY), b2Vec2(windowMaxX, windowMaxY), false);
            player.draw(window);

            // HUD !!CLASS!!
            ui.setSize(window_width, window_height);
            window.setView(ui.getView());

            if(global_isPaused)
            {
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(window_width, window_height));
                rect.setPosition(0,0);
                rect.setFillColor(sf::Color(100, 100, 100, 155));

                window.draw(rect);
            }

            ui.draw(window);


    //        sf::RectangleShape border;
    //        border.setPosition(0.0f, window_height - (window_height*minimapSize + MINIMAP_BORDER_SIZE));
    //        border.setSize(sf::Vector2f(window_width*minimapSize + MINIMAP_BORDER_SIZE, window_height*minimapSize + MINIMAP_BORDER_SIZE));
    //        border.setFillColor(sf::Color(0, 0, 200, 255));
    //
    //        window.draw(border);
    //
    //        // ------------------------------- Minimap -------------------------------
    //
    //        minimap.setSize(MAX_WORLD_WIDTH * BOX_SIZE, MAX_WORLD_WIDTH * BOX_SIZE, window_width, window_height);
    //        minimap.calcViewport(minimapSize);
    //        minimap.setCameraPosition(windowView.getCenter().x, windowView.getCenter().y);
    //        window.setView(minimap.getUpdatedView());
    //
    //        minimap.update(gameEvents);
    //
    //        minimap.draw(window);
    //        gameWorld.draw(window, b2Vec2(-10000, -20000), b2Vec2(10000, 10000), true);
    //        player.draw(window);
    //        minimap.drawCameraWindow(window);

            // Default View again
            window.setView(windowView);
        }

        // Display
        window.display();
    }

    return 0;
}

sf::View updateView(sf::View view)
{
    // Calculate view movement
    int x = view.getCenter().x;
    int y = view.getCenter().y;

    float percX = gameEvents.lastMouseWindow.x / window_width;
    float percY = gameEvents.lastMouseWindow.y / window_height;

    if(gameEvents.isAHeld)
    {
        x -= SCROLL_SPEED;
    }
    else if(percX >= 0.0f && percX <= DEFAULT_SCROLL_BORDER && global_scrollBordersEnabled)
    {
        x -= SCROLL_SPEED_MODIFIER * SCROLL_SPEED;
    }

    if(gameEvents.isDHeld)
    {
        x += SCROLL_SPEED;
    }
    else if(percX >= 1.0f - DEFAULT_SCROLL_BORDER && percX <= 1.0f && global_scrollBordersEnabled)
    {
        x += SCROLL_SPEED_MODIFIER * SCROLL_SPEED;
    }

    if(gameEvents.isWHeld)
    {
        y -= SCROLL_SPEED;
    }
    else if(percY >= 0.0f && percY <= DEFAULT_SCROLL_BORDER && global_scrollBordersEnabled)
    {
        y -= SCROLL_SPEED_MODIFIER * SCROLL_SPEED;
    }

    if(gameEvents.isSHeld)
    {
        y += SCROLL_SPEED;
    }
    else if(percY >= 1.0f - DEFAULT_SCROLL_BORDER && percY <= 1.0f && global_scrollBordersEnabled)
    {
        y += SCROLL_SPEED_MODIFIER * SCROLL_SPEED;
    }

    // Check for horizontal boundaries
//    if(x < window_width/2 - BOX_SIZE/2)
//    {
//        x = window_width/2 - BOX_SIZE/2;
//    }
//
//    if(x > MAX_WORLD_WIDTH * BOX_SIZE - window_width/2 - BOX_SIZE/2)
//    {
//        x = MAX_WORLD_WIDTH * BOX_SIZE - window_width/2 - BOX_SIZE/2;
//    }

    // Check for vertical boundaries
    if(y > MAX_WORLD_HEIGHT * BOX_SIZE + GROUND_STARTING_HEIGHT - window_height/2 - BOX_SIZE/2)
    {
        y = MAX_WORLD_HEIGHT * BOX_SIZE + GROUND_STARTING_HEIGHT - window_height/2 - BOX_SIZE/2;
    }

    // 53 Blocks to the top!

    view.setCenter(x, y);
    return view;
}

void initializeMenu()
{
    // Logos
    menu_logo_bottom = menu.addImage(0, 0, textures["menu_logo_bottom"].getSize().x, textures["menu_logo_bottom"].getSize().y, textures["menu_logo_bottom"]);
    menu_logo = menu.addImage(20, 20, textures["menu_logo"].getSize().x, textures["menu_logo"].getSize().y, textures["menu_logo"]);

    // Resume
    menu_button_resume = menu.addButton(50, 200, textures["menu_button_resume"].getSize().x, textures["menu_button_resume"].getSize().y, textures["menu_button_resume"]);
    menu_button_resume->callbackFunc = &resume;
    menu_button_resume->isDrawn = false;

    // Play
    menu_button_play = menu.addButton(50, 270, textures["menu_button_play"].getSize().x, textures["menu_button_play"].getSize().y, textures["menu_button_play"]);
    menu_button_play->callbackFunc = &play;

    // Instructions
    menu_button_instructions = menu.addButton(50, 340, textures["menu_button_instructions"].getSize().x, textures["menu_button_instructions"].getSize().y, textures["menu_button_instructions"]);
    menu_button_instructions->callbackFunc = &instructions;

    //Quit
    menu_button_quit = menu.addButton(50, 410, textures["menu_button_quit"].getSize().x, textures["menu_button_quit"].getSize().y, textures["menu_button_quit"]);
    menu_button_quit->callbackFunc = &quit;

    // Controls & Goal
    menu_button_instructions_controls = menu.addButton(450, 72, textures["menu_button_controls"].getSize().x, textures["menu_button_controls"].getSize().y, textures["menu_button_controls"]);
    menu_button_instructions_controls->callbackFunc = &instructions_controls;
    menu_button_instructions_controls->enableBackground(true, sf::Color(150, 150, 150, 255), sf::Color::Black);
    menu_button_instructions_controls->isDrawn = false;

    menu_button_instructions_gameplay = menu.addButton(630, 72, textures["menu_button_gameplay"].getSize().x, textures["menu_button_gameplay"].getSize().y, textures["menu_button_gameplay"]);
    menu_button_instructions_gameplay->callbackFunc = &instructions_gameplay;
    menu_button_instructions_gameplay->enableBackground(true, sf::Color(150, 150, 150, 255), sf::Color::Black);
    menu_button_instructions_gameplay->isDrawn = false;
}

void resume()
{
    global_isPaused = false;
    global_isMenu = false;
}

void play()
{
    gameWorld.generateWorld(physicsWorld);
    global_isPaused = false;
    global_isMenu = false;
    menu_button_resume->isDrawn = true;
}

void instructions()
{
    menu_button_instructions_controls->isDrawn = true;
    menu_button_instructions_gameplay->isDrawn = true;
    show_instructions = true;
}

void quit()
{
    window.close();
}

void instructions_controls()
{
    show_instructions_controls = true;
    show_instructions_gameplay = false;
}

void instructions_gameplay()
{
    show_instructions_controls = false;
    show_instructions_gameplay = true;
}

void showInstructions()
{
    sf::RectangleShape background;
    background.setPosition(450, 122);
    background.setSize(sf::Vector2f(800, 365));
    background.setFillColor(sf::Color(150, 150, 150, 255));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(1.0f);

    window.draw(background);
}
void showControls()
{
    sf::Text controls("Left Click on Box: Select\n                  on World: Deselect all boxes\nLeft Dragged: Selection area\n\nRight Click with Selection: Move Command\nRight Held without Selection:\n   on Static Box: Convert to team\n   on Dynamic Box: Push\n\nWASD - Move Camera\nSpace - Attack / Utility\nQ - Show all info\nE - Select all\nR - Autoattack (Normal Boxes only)\nT - Get Box with Charge\n1 - Select Normal Boxes\n2 - Select Magic Boxes\n3 - Select Charge Boxes\n4 - Select all Float Boxes", font_default, 16);
    controls.setPosition(460, 132);
    controls.setColor(sf::Color::White);

    sf::Text controls2("5 - Select all Boom Boxes\n\nY - Disable ally AI\n\nP - Pause game\nZ - Next Music Track\nU - Play / Pause Music\nEscape - Open Menu (Game will be paused)", font_default, 16);
    controls2.setPosition(800, 132);
    controls2.setColor(sf::Color::White);

    window.draw(controls);
    window.draw(controls2);
}

void showGameplay()
{
    sf::Text gameplay("The goal of the game is too reach and touch the orange FoxBox, which will grant your boxes access to the next\n world. Reaching the FoxBox will be no easy task, as it is protected by a huge amount of red Boxes.\n\nTo accomplish this goal, you must help the green Boxes get past their enemies. The Boxes are equipped with a\nbasic AI to move, fight and harvest static boxes. But they have no idea where to go, so you need to guide them.\n\nDifferent Boxes possess different attacks and attributes. Normal Boxes can stomp the ground and enemies.\nCharge Boxes can charge a long way in any direction, dealing huge damage to dynamic boxes.\nFloat Boxes ignore gravity and can drag away other boxes by activating their sticky function. They can also act as\nstatic boxes, so other boxes can use them for advanced movement. Boom Boxes explode in a random radius,\ndealing damage and knocking boxes away. Magic Boxes turn static boxes into dynamic ones, heal allies, turn\nenemies to neutrals, and neutrals to allies. Static Boxes in the world that lose all direct connections\n(up/down/left/right) will become dynamic. Most Boxes also have tag charges, which will be used to turn dynamic\nneutral boxes to your team. The neutral box needs to be touched. The charge is then consumed.\n\nYou can directly influence static and dynamic boxes by right clicking and holding the mouse button. Static Boxes\ncan be turned to allies, and dynamic boxes can be pushed away. But these actions cost resources, which are\ngained by destroying static boxes and enemies. Destroying special Boxes grants more resources. You will also\ngain some resources every few seconds, depending on the amount of boxes you command.", font_default, 16);
    gameplay.setPosition(460, 132);
    gameplay.setColor(sf::Color::White);

    window.draw(gameplay);
}
