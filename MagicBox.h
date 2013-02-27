#ifndef MAGICBOX_H
#define MAGICBOX_H

#include "Box.h"


class MagicBox : public Box
{
    public:
        // ------------------------------- Methods -------------------------------
        MagicBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~MagicBox();

        // ------------------------------- Attributes -------------------------------

    protected:
        // ------------------------------- Methods -------------------------------
        void ai();
        void init();
        void attack();

        // ------------------------------- Attributes -------------------------------
        int healPower;
        int squareRadius;

    private:
};

#endif // MAGICBOX_H
