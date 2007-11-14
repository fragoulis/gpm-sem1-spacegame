#pragma once
#include "VisualDisplayList.h"
#include "Tile3d.h"
#include "Object.h"
#include <map>

using std::map;
using namespace tlib;

typedef map< int, Tile3d* > _Tilemap;

// We define a custom class that will define the spacestation
// in terms of a display list
class SpacestationCorridorsDisplayList : public OCVisualDisplayList
{
private:
    // The number of tiles along any axis
    int m_iNumOfTiles;

    // A single tile's size
    int m_iTileSize;
    
    // The list of tiles
    Tile3d *m_TileList;

    // The list of tiles mapped into an integer, used to quickly find 
    // a wanted tile
    // The key is the index of the tile in an imaginary 3d cube
    _Tilemap m_Tilemap;

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

    /**
     * Returns the address of a tile by its index
     */
    Tile3d* getTile( int i, int j, int k )
    { 
        int index = i + 
                    j * m_iNumOfTiles + 
                    k * m_iNumOfTiles * m_iNumOfTiles;
        
        return m_Tilemap[index];
    }

    /**
     * Returns the tile which an object is in
     */
    Tile3d* getTile( const Object *obj )
    { 
        float fHalfTiles = (float)m_iNumOfTiles * 0.5f;
        Vector3f vIndex = obj->getPos() / (float)m_iTileSize;
        vIndex.addX( fHalfTiles );
        vIndex.addY( fHalfTiles );
        vIndex.subZ( fHalfTiles);
        return getTile( (int)vIndex.x(), (int)vIndex.y(), -(int)vIndex.z() );
    }

}; // end of SpacestationCorridorsDisplayList class
