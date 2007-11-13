#pragma once
#include "VisualDisplayList.h"
using tlib::OCVisualDisplayList;

// We define a custom class that will define the spacestation
// in terms of a display list
class SpacestationDisplayList : public OCVisualDisplayList
{
private:
    // The number of tiles along any axis
    int m_iNumOfTiles;

    // A single tile's size
    int m_iTileSize;
    
public:
    /**
     * Constructor
     */
    SpacestationDisplayList();

    /**
     * Destructor
     */
    virtual ~SpacestationDisplayList();

    /**
     * Builds the cube object with textures using immediate mode
     */
    void buildObject() const;

}; // end of SpacestationDisplayList class
