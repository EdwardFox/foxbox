#include "utility.h"

// Global variable definition
std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::SoundBuffer> soundBuffers;
std::vector<std::string> musicStrings;
bool global_showFriendlyBoxInfo;
sf::Font font_default;
float minimapSize;
int window_width;
int window_height;
bool global_scrollBordersEnabled;
bool global_enableAllyAI;
bool global_isPaused;
bool global_levelComplete;
bool global_isMenu;

sf::Sound box_attack;
sf::Sound box_attack2;
sf::Sound box_dead;
sf::Sound box_contact;
sf::Sound menu_click;
sf::Sound fireworks;
sf::Music music;

// Debug
bool debug_showTargetLines;
bool debug_showTargetArea;
bool debug_showStaticHealth;
bool debug_showInactiveGroundBoxes;
bool debug_global_detonateOnContact;

sf::Texture createTexture(std::string path, bool smooth)
{
    sf::Texture tex;
    tex.loadFromFile(path);
    tex.setSmooth(smooth);

    return tex;
}

void initGlobals()
{
    // Textures
    textures["overlay_allied"] = createTexture("resources/img/overlay_allied.png", true);
    textures["overlay_selected"] = createTexture("resources/img/overlay_selected.png", true);
    textures["overlay_enemy"] = createTexture("resources/img/overlay_enemy.png", true);

    textures["box_ground"] = createTexture("resources/img/box.png", true);
    textures["boombox_ground"] = createTexture("resources/img/boombox.png", true);
    textures["chargebox_ground"] = createTexture("resources/img/chargebox.png", true);
    textures["floatbox_ground"] = createTexture("resources/img/floatbox.png", true);
    textures["magicbox_ground"] = createTexture("resources/img/magicbox.png", true);
    textures["foxbox_ground"] = createTexture("resources/img/foxbox.png", true);

    // Textures: Menu
    textures["menu_logo"] = createTexture("resources/img/logo.png", true);
    textures["menu_logo_bottom"] = createTexture("resources/img/logo_bottom.png", true);
    textures["menu_button_resume"] = createTexture("resources/img/button_resume.png", true);
    textures["menu_button_play"] = createTexture("resources/img/button_play.png", true);
    textures["menu_button_instructions"] = createTexture("resources/img/button_instructions.png", true);
    textures["menu_button_quit"] = createTexture("resources/img/button_quit.png", true);
    textures["menu_button_controls"] = createTexture("resources/img/button_controls.png", true);
    textures["menu_button_gameplay"] = createTexture("resources/img/button_gameplay.png", true);

    // Sounds
    soundBuffers["box_stomp"] = createSound("resources/sounds/box_stomp.aif");
    soundBuffers["box_stomp2"] = createSound("resources/sounds/box_stomp_02.wav");
    soundBuffers["box_dead"] = createSound("resources/sounds/box_dead.wav");
    soundBuffers["box_contact"] = createSound("resources/sounds/box_contact.wav");
    soundBuffers["menu_click"] = createSound("resources/sounds/menu_click.wav");
    soundBuffers["fireworks"] = createSound("resources/sounds/finale_fireworks.wav");

    box_attack.setBuffer(soundBuffers["box_stomp"]);
    box_attack.setVolume(25.0f);
    box_attack.setPitch(3.0f);
    box_attack.setAttenuation(100.0f);

    box_attack2.setBuffer(soundBuffers["box_stomp2"]);
    box_attack2.setVolume(45.0f);
    box_attack2.setAttenuation(100.0f);

    box_dead.setBuffer(soundBuffers["box_dead"]);
    box_dead.setVolume(25.0f);
    box_dead.setAttenuation(100.0f);

    box_contact.setBuffer(soundBuffers["box_contact"]);
    box_contact.setVolume(45.0f);
    box_contact.setAttenuation(100.0f);

    menu_click.setBuffer(soundBuffers["menu_click"]);
    menu_click.setVolume(45.0f);

    fireworks.setBuffer(soundBuffers["fireworks"]);
    fireworks.setVolume(45.0f);

    // Music
    musicStrings.push_back("resources/music/Boom_Kitty-The_Floor.ogg");
    musicStrings.push_back("resources/music/cautious-path.ogg");
    musicStrings.push_back("resources/music/heart-of-the-sea.ogg");
    musicStrings.push_back("resources/music/Matt_Porter-Doors.ogg");
    musicStrings.push_back("resources/music/The_Long_Summer.ogg");

    music.setVolume(20.0f);
    music.setLoop(false);
    music.setPitch(1);

    // Debug
    debug_showStaticHealth = false;
    debug_showTargetArea = false;
    debug_showTargetLines = false;
    debug_global_detonateOnContact = false;
    debug_showInactiveGroundBoxes = true;

    // Global
    global_showFriendlyBoxInfo = false;
    font_default = font_default.getDefaultFont();
    minimapSize = 0.3f;
    window_width = 1280;
    window_height = 720;
    global_scrollBordersEnabled = false;
    global_enableAllyAI = true;
    global_isPaused = false;
    global_levelComplete = false;
    global_isMenu = true;
}

std::string getProperType(std::string type)
{
    if(type == "box")
    {
        return "Box";
    }

    if(type == "boombox")
    {
        return "BoomBox";
    }

    if(type == "chargebox")
    {
        return "ChargeBox";
    }

    if(type == "floatbox")
    {
        return "FloatBox";
    }

    if(type == "magicbox")
    {
        return "MagicBox";
    }

    if(type == "foxbox")
    {
        return "FoxBox";
    }

    return "Not specified. (Update utility function)";
}

b2Vec2 getGroundCoordsByCoordinates(int x, int y)
{
    int fullX = (x + (int)BOX_SIZE/2) / (int)BOX_SIZE;
    int restX = (x + (int)BOX_SIZE/2) % (int)BOX_SIZE;

    if(restX > 0)
    {
        fullX++;
    }

    fullX--;
    fullX *= BOX_SIZE;

    int fullY = (y + (int)BOX_SIZE/2) / (int)BOX_SIZE;
    int restY = (y + (int)BOX_SIZE/2) % (int)BOX_SIZE;

    if(restY > 0)
    {
        fullY++;
    }

    fullY--;
    fullY *= BOX_SIZE;

    b2Vec2 result(fullX, fullY);

    return result;
}

std::string getIndexByGroundCoordinates(int x, int y)
{
    std::stringstream result;
    result << x << "x" << y;
    return result.str();
}

sf::SoundBuffer createSound(std::string path)
{
    sf::SoundBuffer buffer;
    buffer.loadFromFile(path);
    return buffer;
}
