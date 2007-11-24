#pragma once
#include "Object.h"
using tlib::Object;

class TileObject : public Object
{
private:
    // Flags whether an object should continue to render or not
    bool m_bActive;

public:
    /**
     * Constructor
     */
    TileObject(): m_bActive(true) {}

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
    void setPosFromIndex( int ijk[], float *vfOffset = 0 );

    /**
     * Deactivates the object
     */
    void deactivate() { m_bActive = false; }

    /** 
     * Returs the active flag
     */
    bool isActive() const { return m_bActive; }

};