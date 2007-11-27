#pragma once
#include "CollisionBSphere.h"
#include "Object.h"

class Tile3d;

namespace tlib 
{
    /**
     * This component not only does it have ability to detect collisions
     * with other bounding boxes but it can also detect collision with 
     * the spacestation corridors
     */
    class OCCollisionDynamicBSphere : public OCCollisionBSphere
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
        OCCollisionDynamicBSphere();
        OCCollisionDynamicBSphere( float fRadius );

        /**
         * Destructor
         */
        virtual ~OCCollisionDynamicBSphere(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("collisiondynamicbsphere"); 
        }

        /**
         * It checks the owner object with the occupant object
         * of the tile
         */
        bool checkWithObject( Vector3f &vCollDir ) const;

        /**
         * Checks collision with spacestation corridors
         */
        bool checkWithTile( Vector3f &vCollDir ) const;

        /**
         * Uses the owner's position to see in which tile the 
         * owner is and returns true if owner is actually
         * within a tile
         */
        bool readTile();

        /**
         * Calls the check function and calls each object's response
         * component if collision was detected
         */
        void actOnCollision() const;

    private:
        /**
         * Initializes member variables from the configuration file
         */
        void _readConfig();

    }; // end of OCCollisionDynamicBSphere class

} // end of namespace tlib