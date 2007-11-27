#pragma once
#include "VisualDisplayList.h"
#include "Object.h"
using namespace tlib;

// We define a custom class that will define the spacestation
// in terms of a display list
class SpacestationCorridorsDisplayList : public OCVisualDisplayList
{
public:
    /**
     * Constructor
     */
    SpacestationCorridorsDisplayList();

    /**
     * Destructor
     */
    virtual ~SpacestationCorridorsDisplayList();

    /**
     * Reads the station data from file and saves them to an
     * array
     */
    bool readMap( const char *filename );

    /**
     * Builds the cube object with textures using immediate mode
     */
    void buildObject() const;

}; // end of SpacestationCorridorsDisplayList class
