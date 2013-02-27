#ifndef BOX_H
#define BOX_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "utility.h"
#include "Events.h"
#include "TargetArea.h"
#include "Timer.h"

class Box
{
    public:
        // ------------------------------- Methods -------------------------------
        Box(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~Box();

        void setTexture(sf::Texture &tex);
        void setOverlayTexture(sf::Texture &tex);
        void update(Events gameEvents);
        void draw(sf::RenderWindow& window);
        void startContact(Box* box, bool isDynamic);
        void endContact(Box* box, bool isDynamic);
        void setLatestImpulse(float imp);
        void updateTexture();
        void notifyNeighbours();
        void notifyContactsOnConversion(bool staticToDynamic);
        void notifyDynamics();
        void reduceCharges(int amount);
        void displayAdditionals(sf::RenderWindow& window);
        void setBodyActive(bool active);
        void takeHit(float dmg, int team);
        void heal(float health);
        void resetAiTarget();
        void resetAttack();
        void stop();

        virtual void changeTeam(int team);
        virtual void createBox(b2World& physicsWorld, b2Vec2 position, b2Vec2 size, float density, float friction, bool isDynamic, uint16 categoryBits, uint16 maskBits);

        sf::Sprite* getSprite();
        b2Body* getBody();

        // ------------------------------- Attributes -------------------------------
        bool isActive;
        bool hasActiveTargetArea;
        bool isJumping;
        bool isDead;
        bool isAttacking;
        bool isNotified;
        bool isFlaggedForConversion;
        bool isConverted;
        bool isInfo;
        bool isSticky;
        bool repeatAttack;

        std::vector<Box*> contactDynamicList;
        std::vector<Box*> contactStaticList;

        int team;
        int tagCharges;
        int lastDamageReceivedFrom;

        TargetArea target;

        float speed;
        float stopSpeed;
        float jumpHeight;
        float lastImpulseReceived;

        std::string type;

        neighbour neighbours[9];

        Box* stickyMaster;
        Box* floatMaster;
        std::vector<Box*> targetMasters;

    protected:
        // ------------------------------- Methods -------------------------------
        bool isSelected(int mouseX, int mouseY);
        bool isInMotion();
        bool groundBoxExists(const std::string id);

        void jump(float jumpHeight, float speed);
        void dampenedJump();
        void processImpulse();
        void checkHeight();
        void checkNotifications();
        void checkForConversion();
        void calcInfoBoxPosition();
        void displayInfo(sf::RenderWindow& window);
        void displayMarker(sf::RenderWindow& window);
        void addBoxInfo(int id, std::string text);
        void moveToTarget(float x, float y, float radius, bool withStopCondition, float speed);
        void playRandomAttackSound();

        Box* getDynamicBoxByCoordinates(int x, int y);

        int addBoxInfoLine();

        virtual void ai();
        virtual void init();
        virtual void attack();
        virtual void updateInfo();
        virtual void moveToTargetArea();


        // ------------------------------- Attributes -------------------------------
        int health;

        float stompJumpForce;
        float stompForce;
        bool attackStarted;
        Timer attackTimer;
        Timer aiAttackTimer;
        float damageResistance;
        float attackDamage;
        int attackRange;

        Timer contactSoundTimer;
        Timer hitTimer;
        Timer hitNumberTimer;
        sf::Text text_damageTaken;

        b2Vec2 preJumpPosition;
        b2Vec2 lastMouse;

        Box* aiTarget;

        b2Body* body;
        sf::Sprite sprite;
        sf::Sprite overlaySprite;

        std::map<std::string, Box*>* groundBoxes;
        std::vector<Box*>* boxes;

        // Display info
        std::vector<sf::Text> text_info;
        sf::RectangleShape text_background;
        float text_topLeftX;
        float text_topLeftY;

    private:
};

#endif // BOX_H
