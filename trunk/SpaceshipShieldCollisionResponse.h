#pragma once
#include "CollisionResponse.h"
using tlib::IOCCollisionResponse;
using tlib::Vector3f;

class SpaceshipShieldCollisionResponse : public IOCCollisionResponse
{
public:
    /**
     * Destructor
     */
    virtual ~SpaceshipShieldCollisionResponse() {}

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("spaceshipshieldcollisionresponse"); 
    }

    /**
     * Responds to the collision
     * The parameter is the collision vector
     */
    void respond() const;
};