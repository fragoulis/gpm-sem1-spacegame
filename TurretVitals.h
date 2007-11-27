#pragma once
#include "VitalsHealth.h"
using tlib::OCVitalsHealth;

class TurretVitals : public OCVitalsHealth
{
public:
    /**
     * Constructor
     */
    TurretVitals( int iMaxHealth );
    
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("turretvitals"); 
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