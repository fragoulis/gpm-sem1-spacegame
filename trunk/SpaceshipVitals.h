#pragma once
#include "VitalsLives.h"
using tlib::OCVitalsLives;

class SpaceshipVitals : public OCVitalsLives
{
public:
    /**
     * Constructor
     */
    SpaceshipVitals( int iMaxLives, int iMaxHealth );
    
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("spaceshipvitals"); 
    }

    /**
     * This is called on an object's death
     */
    virtual void onKill();

    /**
     * This is called on an object's revival
     */
    virtual void onRevive();

}; // end of TurretVitals class