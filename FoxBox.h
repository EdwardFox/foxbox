#ifndef FOXBOX_H
#define FOXBOX_H

#include "Box.h"


class FoxBox : public Box
{
    public:
        FoxBox(std::map<std::string, Box*>* groundBoxes, std::vector<Box*>* boxes);
        virtual ~FoxBox();

        void changeTeam(int team);

    protected:
        void ai();
        void init();

    private:
};

#endif // FOXBOX_H
