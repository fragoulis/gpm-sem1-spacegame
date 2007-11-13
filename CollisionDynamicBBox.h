#pragma once
#include "CollisionBBox.h"
#include "Object.h"

class Tile3d;
class SpacestationCorridors;

namespace tlib 
{
    /**
     * This component not only does it have ability to detect collisions
     * with other bounding boxes but it can also detect collision with 
     * the spacestation corridors
     */
    class OCCollisionDynamicBBox : public OCCollisionBBox
    {
    private:
        // This pointer holds the address of the tile the spaceship
        // is in
        Tile3d *m_CurTile;

        // The tile size
        int m_iTileSize;

        // The halh number of tiles per side
        int m_iHalfNumOfTiles;

    public:
        /**
         * Constructor
         */
        OCCollisionDynamicBBox();
        OCCollisionDynamicBBox( const Vector3f& );

        /**
         * Destructor
         */
        virtual ~OCCollisionDynamicBBox(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("collisiondynamicbbox"); 
        }

        /**
         * Checks collision with spacestation corridors
         */
        bool checkWithTile( Vector3f& vCollDir ) const;

        /**
         * Uses the owner's position to see in which tile the 
         * owner is and returns true if owner is actually
         * within a tile
         */
        bool readTile( SpacestationCorridors& oCor );

    private:
        /**
         * Initializes member variables from the configuration file
         */
        void _readConfig();

    }; // end of OCCollisionDynamicBBox class

} // end of namespace tlib