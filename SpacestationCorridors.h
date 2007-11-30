
#pragma once

#include "Object.h"
using namespace tlib;

/**
 * The Spacestation class
 * Contains a custom subclass of the DisplayList component to handle
 * its compiled rendering code and Tile structure that holds information 
 * abour the corridors
 */
class SpacestationCorridors : public Object
{
public:
    /**
     * Constructor
     */
    SpacestationCorridors();

    /**
     * Creates the spacestation
     */
    void init();

}; // end of Spacestation class