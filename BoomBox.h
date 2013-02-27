#ifndef BOOMBOX_H
#define BOOMBOX_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Box.h"
#include "utility.h"

class BoomBox : public Box
{
    public:
        // ------------------------------- Methods -------------------------------
        BoomBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~BoomBox();

        // ------------------------------- Attributes -------------------------------
        int radius;
        int powerMod;
        float rotationMod;

        bool detonateOnContact;
    protected:
        // ------------------------------- Methods -------------------------------
        void ai();
        void init();
        void attack();
        void updateInfo();

        // ------------------------------- Attributes -------------------------------
        int text_id_radius;
    private:
};

#endif // BOOMBOX_H
