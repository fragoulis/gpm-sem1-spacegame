#pragma once
#include "Object.h"
using tlib::Object;

class TileObject : public Object
{
public:
    /**
     * Destructor
     */
    virtual ~TileObject() {}

    /**
     * Takes the index of the tile in which an object is created
     * in order to set the object's position according to the tile's
     * position.
     * Is also takes a string identifier of the new object in order
     * to search the configuration file for an offset to that position
     */
    void setPosFromIndex( const int *ijk, float *vfOffset = 0 );

};