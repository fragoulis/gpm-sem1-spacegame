#pragma once
#include "CollisionResponse.h"
using tlib::IOCCollisionResponse;
using tlib::Vector3f;

class ReactorCollisionResponse : public IOCCollisionResponse
{
public:
    /**
     * Destructor
     */
    virtual ~ReactorCollisionResponse() {}

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("reactorcollisionresponse"); 
    }

    /**
     * Responds to the collision
     * The parameter is the collision vector
     */
    void respond();
};