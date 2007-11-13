#pragma once
#include "Object.h"
using namespace tlib;

/**
 * The Spacestation class
 */
class Spacestation : public Object
{
public:
    /**
     * Initializes the Spacestation by reading the configuration file
     * and setting components
     * This can also be called from within the constructor
     * NOTE: Error/Action logger doesn't not log contructor messages
     */
    void setup();

}; // end of Spacestation class