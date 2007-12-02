#pragma once
#include "VitalsLives.h"
using tlib::OCVitalsLives;

class SpaceshipShieldVitals : public OCVitalsLives
{
public:
    /**
     * Constructor
     */
    SpaceshipShieldVitals( int iMaxLives, int iMaxHealth );
    
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("SpaceshipShieldVitals"); 
    }

    /**
     * This is called on an object's death
     */
    virtual void onKill();

    /**
     * This is called on an object's revival
     */
    virtual void onRevive();

}; // end of SpaceshipShieldVitals class