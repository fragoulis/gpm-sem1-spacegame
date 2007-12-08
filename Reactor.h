#pragma once
#include "Object.h"
using namespace tlib;

class Timer;

/**
 * This is our Reactor object
 */
class Reactor : public Object
{
private:
    Timer *m_AnimTimer, *m_GlowTimer;
    float m_fRotFactor;

public:
    /**
     * Constructor
     */
    Reactor();

    /**
     * Accessors
     */
    Timer* getGlowTimer() { return m_GlowTimer; }

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

    /**
     * Updates the reactor
     */
    void update();

}; // end Reactor class