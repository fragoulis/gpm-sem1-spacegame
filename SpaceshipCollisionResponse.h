#pragma once
#include "CollisionResponse.h"
using tlib::IOCCollisionResponse;
using tlib::Vector3f;

class SpaceshipCollisionResponse : public IOCCollisionResponse
{
public:
    /**
     * Destructor
     */
    virtual ~SpaceshipCollisionResponse() {}

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("spaceshipcollisionresponse"); 
    }

    /**
     * Responds to the collision
     * The parameter is the collision vector
     */
    void respond( const Vector3f &vCollVector ) const;
};