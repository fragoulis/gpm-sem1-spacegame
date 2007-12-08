#pragma once
#include "GXJoystick.h"
using tlib::OCGXJoystick;

class SpaceshipJoystick : public OCGXJoystick
{
public:
    /**
     * Here we map the controls to our spacecraft
     */
    void update();

}; // end of OCSpaceshipKeyboard class