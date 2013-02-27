#ifndef FLOATBOX_H
#define FLOATBOX_H

#include "Box.h"

class FloatBox : public Box
{
    public:
        // ------------------------------- Methods -------------------------------
        FloatBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~FloatBox();

        void changeTeam(int team);
        void createBox(b2World& world, b2Vec2 position, b2Vec2 size, float density, float friction, bool isDynamic, uint16 categoryBits, uint16 maskBits);

        // ------------------------------- Attributes -------------------------------

    protected:
        // ------------------------------- Methods -------------------------------
        void ai();
        void init();
        void attack();
        void updateInfo();
        void moveToTargetArea();


        // ------------------------------- Attributes -------------------------------
        float floatNeutralSpeed;

        int text_id_sticky;

        b2Vec2 targetSpeed;

        // AI
        int searchRange;
        int searchRangeVertical;
        int firstRandomAltitude;
        int secondRandomAltitude;
        int dropRange;

        b2Vec2 returnPos;

        bool hasGainedAltitude;
        bool hasGainedAltitudeAgain;
        bool hasTargetCaptured;
};

#endif // FLOATBOX_H
