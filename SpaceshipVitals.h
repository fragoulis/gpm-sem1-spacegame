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
    void onKill();

    /**
     * This is called on an object's revival
     */
    void onRevive();

    /**
     * This is called when an dying object
     * is out of lives
     */
    void onEndOfLives();

}; // end of SpaceshipVitals class