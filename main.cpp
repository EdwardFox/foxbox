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

// Variables
int antiAliasLevel = 8;
std::string name = "Foxbox";

b2Vec2 gravity(0.0f, 9.8f);
b2World physicsWorld(gravity, true);
World gameWorld;
Events gameEvents;
Player player;
UI ui;

// Functions
sf::View updateView(sf::View view);

int main()
{
    // Initialize globals
    initGlobals();

    // Create window
    sf::ContextSettings settings;
    settings.antialiasingLevel = antiAliasLevel;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), name, sf::Style::Default, settings);
    window.setFramerateLimit(FRAMES_PER_SECOND);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // Camera view
    sf::View windowView;

    // UI
    ui.init();

    // Minimap
//    Minimap minimap;

    // Create & Set contactlistener
    BoxContactListener boxContactListener;
    physicsWorld.SetContactListener(&boxContactListener);

    // Build world
    gameWorld.generateWorld(physicsWorld);

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

        // Clear window
        window.clear(sf::Color(150, 150, 150, 255));

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
