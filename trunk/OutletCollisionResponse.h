#pragma once
#include "CollisionResponse.h"
using tlib::IOCCollisionResponse;
using tlib::Vector3f;

class OutletCollisionResponse : public IOCCollisionResponse
{
private:


public:
    /**
     * Destructor
     */
    virtual ~OutletCollisionResponse() {}

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("outletcollisionresponse"); 
    }

    /**
     * Responds to the collision
     * The parameter is the collision vector
     */
    void respond() const;
};