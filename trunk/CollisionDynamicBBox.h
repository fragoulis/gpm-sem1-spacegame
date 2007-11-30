#pragma once
#include "CollisionBBox.h"
#include "Object.h"

class Tile3d;

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
         * Returns the tile pointer
         */
        const Tile3d& getTile() const { return *m_CurTile; }
        Tile3d* getTile() { return m_CurTile; }
        void setTile( Tile3d *tile ) { m_CurTile = tile; }

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
        void checkCollision() const;

    private:
        /**
         * Initializes member variables from the configuration file
         */
        void _readConfig();

    }; // end of OCCollisionDynamicBBox class

} // end of namespace tlib