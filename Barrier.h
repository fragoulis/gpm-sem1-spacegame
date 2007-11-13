#pragma once
#include "Object.h"
using tlib::Object;
using tlib::Vector3f;

class Barrier : public Object
{
private:
    // This flag tells us whether the barrier is 'harmfull' or not
    bool bIsActive;

public:
    /**
     * Constructor
     */
    Barrier();
    Barrier( const Vector3f& vBBox );

    /**
     * Takes the index of the tile in which is this barrier 
     * in an array and positions it once and for all
     */
    void setPosFromIndex( int ijk[] );
};