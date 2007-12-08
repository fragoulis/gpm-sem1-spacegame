#pragma once
#include "CollisionResponse.h"
using tlib::IOCCollisionResponse;
using tlib::Vector3f;

class TurretCollisionResponse : public IOCCollisionResponse
{
public:
    /**
     * Destructor
     */
    virtual ~TurretCollisionResponse() {}

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("turretcollisionresponse"); 
    }

    /**
     * Responds to the collision
     * The parameter is the collision vector
     */
    void respond();
};