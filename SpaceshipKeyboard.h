
#pragma once

#include "keyboard.h"
using tlib::OCKeyboard;

class SpaceshipKeyboard : public OCKeyboard
{
public:
    /**
     * Here we map the controls to our spacecraft
     */
    void update();

}; // end of OCSpaceshipKeyboard class