
#pragma once

#include "keyboard.h"
using tlib::OCKeyboard;

class OCSpaceshipKeyboard : public OCKeyboard
{
public:
    /**
     * Here we map the controls to our spacecraft
     */
    void update() const;

}; // end of OCSpaceshipKeyboard class