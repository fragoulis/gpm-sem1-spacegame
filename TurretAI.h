#pragma once
#include "Object.h"
#include "AI.h"
using tlib::IOCAI;
using tlib::Object;

class TurretAI : public IOCAI
{
private:
    // The target object
    Object *m_oTarget;

    // The rotation factor
    float m_fRotFactor;

public:
    /**
     * Constructor
     */
    TurretAI( Object *obj );
    
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("turretai"); 
    }

    /**
     * Updates the object's transformation and acts
     * on it according to some predefined conditions
     */
    void update( Object *oOwner );
    
}; // end of TurretAI class