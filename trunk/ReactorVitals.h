#pragma once
#include "VitalsHealth.h"
using tlib::OCVitalsHealth;

class ReactorVitals : public OCVitalsHealth
{
public:
    /**
     * Constructor
     */
    ReactorVitals( int iMaxHealth );
    
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("reactorvitals"); 
    }

    /**
     * This is called on an object's death
     */
    void onKill();

    /**
     * This is called on an object's revival
     */
    void onRevive();

}; // end of ReactorVitals class