#ifndef CHARGEBOX_H
#define CHARGEBOX_H

#include "Box.h"


class ChargeBox : public Box
{
    public:
        // ------------------------------- Methods -------------------------------
        ChargeBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~ChargeBox();

        // ------------------------------- Attributes -------------------------------

    protected:
        // ------------------------------- Methods -------------------------------
        void ai();
        void init();
        void attack();
        void updateInfo();

        // ------------------------------- Attributes -------------------------------
        int text_id_attackReady;
        Timer rechargeTimer;

        float speed;

        b2Vec2 targetSpeed;
        b2Vec2 conversionTarget;

        bool attackReady;
        bool attackStarted;
        bool attackStopped;
        bool dynamicTargetHit;

    private:
};

#endif // CHARGEBOX_H
