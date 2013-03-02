#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

// Constants
const float SCALE = 32.0f;
const int FRAMES_PER_SECOND = 60;
const float TIME_STEP = 1.0f/FRAMES_PER_SECOND;
const int VELOCITY_ITERATIONS = 8.0f;
const int POSITION_ITERATIONS = 3.0f;
const int SCROLL_SPEED = 16;
const float SCROLL_SPEED_MODIFIER = 1.0f;
const float DEFAULT_SCROLL_BORDER = 0.05f;

const float BOX_SIZE = 32.0f;
const float GIB_SIZE = 8.0f;
const int GIB_AMOUNT = 4;

const int MAX_WORLD_HEIGHT = 40;
const int MAX_WORLD_WIDTH = 200;
const int MAX_SKY_HEIGHT = 5000;
const int GROUND_STARTING_HEIGHT = 352;
const float IMPULSE_DAMAGE_MULTIPLIER = 0.03;
const float IMPULSE_EFFECT_THRESHOLD = 300.0f;
const int DEFAULT_TEXT_SIZE = 12;
const int DEFAULT_LINE_HEIGHT = DEFAULT_TEXT_SIZE + 3;
const int DEFAULT_TEXT_BOX_PADDING = 5;
const int DEFAULT_BOX_INFO_DISTANCE = 15;

const short int TEAM_NEUTRAL = 0;
const short int TEAM_ALLY = 1;
const short int TEAM_ENEMY = 2;

// HUD
const int MINIMAP_BORDER_SIZE = 10;

// World generation | 10 = 1%
const int WORLD_CHANCE_CANYON = 1000;
const int WORLD_AMOUNT_FLYING_ISLANDS = 3;
const int WORLD_VARY_AMOUNT_FLYING_ISLANDS = 3;

const int WORLD_CHANCE_BOOMBOX = 5;
const int WORLD_CHANCE_BOOMBOX_ENEMY = 50;
const int WORLD_CHANCE_CHARGEBOX = 10;
const int WORLD_CHANCE_FLOATBOX = 15;
const int WORLD_CHANCE_FLOATBOX_ENEMY = 50;
const int WORLD_CHANCE_MAGICBOX = 25;
const int WORLD_CHANCE_MAGICBOX_ENEMY = 25;

// BOX VALUES
const int BOX_VALUE = 2;
const int CHARGEBOX_VALUE = 3;
const int MAGICBOX_VALUE = 3;
const int FLOATBOX_VALUE = 4;
const int BOOMBOX_VALUE = 5;

// Resources
const int BOX_RELEASE_COST = 200;

// Global variable declaration
extern std::map<std::string, sf::Texture> textures;
extern std::map<std::string, sf::SoundBuffer> soundBuffers;
extern std::vector<std::string> musicStrings;
extern bool global_showFriendlyBoxInfo;
extern sf::Font font_default;
extern float minimapSize;
extern int window_width;
extern int window_height;
extern bool global_scrollBordersEnabled;
extern bool global_enableAllyAI;
extern bool global_isPaused;
extern bool global_levelComplete;
extern bool global_isMenu;

extern sf::Sound box_attack;
extern sf::Sound box_attack2;
extern sf::Sound box_dead;
extern sf::Sound box_contact;
extern sf::Sound menu_click;
extern sf::Music music;

// Debug
extern bool debug_global_detonateOnContact;
extern bool debug_showTargetLines;
extern bool debug_showTargetArea;
extern bool debug_showStaticHealth;
extern bool debug_showInactiveGroundBoxes;

// Structs
struct neighbour
{
    bool exists;
    int x;
    int y;
    std::string index;
};

enum boxCategory
{
    BOX_STATIC  = 0x0001,
    BOX_DYNAMIC = 0x0002,
    BOX_GIB     = 0x0004,
};

enum eventLayers
{
    LAYER_NONE  = 0,
    LAYER_MENU  = 1,
    LAYER_GAME  = 2,
    LAYER_HUD   = 3,
};

// Function definitions
sf::Texture createTexture(std::string path, bool smooth);
sf::SoundBuffer createSound(std::string path);

b2Vec2 getGroundCoordsByCoordinates(int x, int y);

std::string getIndexByGroundCoordinates(int x, int y);
std::string getProperType(std::string type);

void initGlobals();



/* ------------------------------- Comment templates -------------------------------
 *
 * Fill lines: -------------------------------
 */

#endif // UTILITY_H_INCLUDED


