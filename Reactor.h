#pragma once
#include "Object.h"
using namespace tlib;

/**
 * This is our Reactor object
 */
class Reactor : public Object
{
private:
    float m_fRotFactor;

public:
    /**
     * Constructor
     */
    Reactor();

    /**
     * Initializes the spaceship by reading the configuration file
     * and setting components
     * This can also be called from within the constructor
     * NOTE: Error/Action logger doesn't not log contructor messages
     */
    void init();

    /**
     * Renders the reactor core object
     */
    void render();

}; // end Reactor class