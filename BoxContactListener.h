#ifndef BOXCONTACTLISTENER_H
#define BOXCONTACTLISTENER_H

#include <iostream>
#include <Box2D/Box2D.h>
#include "Box.h"

class BoxContactListener : public b2ContactListener
{
    public:
        BoxContactListener();
        virtual ~BoxContactListener();

        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    protected:
    private:
        void checkTeamConversion(Box* box1, Box* box2);
};

#endif // BOXCONTACTLISTENER_H
