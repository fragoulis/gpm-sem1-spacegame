#pragma once
#include "TileObject.h"
using tlib::Vector3f;

class Barrier : public TileObject
{
public:
    /**
     * Constructor
     */
    Barrier();
    Barrier( const Vector3f& vBBox );

    /**
     * Destructor
     */
    virtual ~Barrier() {}
};