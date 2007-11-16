#pragma once
#include "Barrier.h"
#include "Blade.h"
#include "BladePoll.h"
#include "BladeBase.h"

class RotatingBlade: public Barrier
{
public:
    // This indicates how much should the barrier move before
    // it goes out of sight [basicaly this is height of the bbox]
    static float DistToDie;

private:
    // The blade of the system
    Blade m_oBlade;

    // The base of the system
    BladePoll m_oPoll;
    BladeBase m_oBase;

public:
    /**
     * Constructor
     */
    RotatingBlade();

    /**
     * Accessor methods for the system's objects
     * Note: there are returning non-const references because I have not
     * found a way around not doing that for the moment
     */
    Blade& getBlade() { return m_oBlade; }
    BladePoll& getBladePoll() { return m_oPoll; }
    BladeBase& getBladeBase() { return m_oBase; }

    /**
     * Renders the whole blade system
     */
    void render();

};