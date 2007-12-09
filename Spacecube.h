#pragma once
#include "Object.h"
using tlib::Object;

class Spacecube : public Object
{
public:
    /**
     * Constructor
     */
    void init();

    /**
     * Renders the spacecube
     */
    void render();
};