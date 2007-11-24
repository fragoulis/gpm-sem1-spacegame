#include "CollisionDynamicBBox.h"
#include "Tile3d.h"
#include "SpacestationCorridors.h"
#include "SpacestationCorridorsDisplayList.h"
#include "CollisionResponse.h"
#include "Config.h"

namespace tlib 
{

    OCCollisionDynamicBBox::OCCollisionDynamicBBox(): 
      m_CurTile(0) 
      {
        _readConfig();
    }
    OCCollisionDynamicBBox::OCCollisionDynamicBBox( const Vector3f& vBBox ): 
      m_CurTile(0),
      OCCollisionBBox( vBBox )
    {
        _readConfig();
    }

    // ------------------------------------------------------------------------
    void OCCollisionDynamicBBox::_readConfig()
    {
        Config cfg("config.txt");
        cfg.loadBlock("spacestation");

        int iNumOfTiles;
        cfg.getInt("tile_size"  ,&m_iTileSize);
        cfg.getInt("tiles"      ,&iNumOfTiles);
        m_iHalfNumOfTiles = (int)(iNumOfTiles * 0.5f);
    }

    // ------------------------------------------------------------------------
    void OCCollisionDynamicBBox::actOnCollision() const
    {
        // The resulting collision vector
        Vector3f vCollDir;

        // Get the owner object's response component
        IOCCollisionResponse *cOwnerRes = 
            (IOCCollisionResponse*)m_oOwner->getComponent("collisionresponse");

        // Check collision with the spacestation corridors
        if( checkWithTile( vCollDir ) ) {
            cOwnerRes->respond( vCollDir );
        }

        // Check with tile's occupier object
        if( checkWithObject( vCollDir ) ) {
            // Get the other object's response component
            // Note: that not all object's have response components
            Object *oOcc = m_CurTile->getOccupant();
            IOCCollisionResponse *cObjRes = 
                (IOCCollisionResponse*)oOcc->getComponent("collisionresponse");

            cOwnerRes->respond( vCollDir );
            if( cObjRes )
                cObjRes->respond( vCollDir );
        }
    }

    // ------------------------------------------------------------------------
    bool OCCollisionDynamicBBox::checkWithObject( Vector3f& vCollDir ) const
    {
        if( m_CurTile->getOccupant() )
            return check( m_CurTile->getOccupant(), vCollDir );

        return false;
    }
    
    // ------------------------------------------------------------------------
    bool OCCollisionDynamicBBox::checkWithTile( Vector3f& vCollDir ) const
    {
        bool isCollision = false;
        float vfOverlap[] = { 0.0f, 0.0f, 0.0f };

        // Owner object's position shorthand
        const Vector3f& vPos = getOwner()->getPos();
        
        // Here we check for collision with each of the tile's planes
        if( m_CurTile->getType() & TW_LEFT ) {
            // Check collision with the left plane
            int x = ( m_CurTile->i - m_iHalfNumOfTiles ) * m_iTileSize;
            float fOverlapX = m_BBox.x() + (float)x - vPos.x();
            if( fOverlapX > 0.0f ) {
                vfOverlap[0] = fOverlapX;
                isCollision = true;
            }
        } 
        
        if( m_CurTile->getType() & TW_RIGHT ) {
            // Check collision with the right plane
            int x = ( ( m_CurTile->i + 1 ) - m_iHalfNumOfTiles ) * m_iTileSize;
            float fOverlapX = m_BBox.x() + vPos.x() - (float)x;
            if( fOverlapX > 0.0f ) {
                vfOverlap[0] = -fOverlapX;
                isCollision = true;
            }
        }

        if( m_CurTile->getType() & TW_BOTTOM ) {
            // Check collision with the bottom plane
            int y = ( m_CurTile->j - m_iHalfNumOfTiles ) * m_iTileSize;
            float fOverlapY = m_BBox.y() + (float)y - vPos.y();
            if( fOverlapY > 0.0f ) {
                vfOverlap[1] = fOverlapY;
                isCollision = true;
            }
        }
        
        if( m_CurTile->getType() & TW_TOP ) {
            // Check collision with the top plane
            int y = ( ( m_CurTile->j + 1 ) - m_iHalfNumOfTiles ) * m_iTileSize;
            float fOverlapY = m_BBox.y() + vPos.y() - (float)y;
            if( fOverlapY > 0.0f ) {
                vfOverlap[1] = -fOverlapY;
                isCollision = true;
            }
        }
        
        if( m_CurTile->getType() & TW_BACK ) {
            // Check collision with the front plane
            int z = ( m_iHalfNumOfTiles - m_CurTile->k ) * m_iTileSize;
            float fOverlapZ = m_BBox.z() + vPos.z() - (float)z;
            if( fOverlapZ > 0.0f ) {
                vfOverlap[2] = -fOverlapZ;
                isCollision = true;
            } 
        }

        if( m_CurTile->getType() & TW_FRONT ) {
            // Check collision with the front plane
            int z = ( m_iHalfNumOfTiles - ( m_CurTile->k + 1 ) ) * m_iTileSize;
            float fOverlapZ = m_BBox.z() + (float)z - vPos.z();
            if( fOverlapZ > 0.0f ) {
                vfOverlap[2] = fOverlapZ;
                isCollision = true;
            }
        }

        // Set the collision vector
        vCollDir.xyz( vfOverlap );

        // Return collision flag
        return isCollision;
    }

    // ------------------------------------------------------------------------
    bool OCCollisionDynamicBBox::readTile( SpacestationCorridors& oCor )
    {
        // Get corridor's [visual] display list component
        SpacestationCorridorsDisplayList *cSCDL = 
            (SpacestationCorridorsDisplayList*)oCor.getComponent("visual");

        // Save the tile in which the object is
        m_CurTile = cSCDL->getTile( getOwner()->getPos() );
        
        return (0==m_CurTile)?0:1;
    }

} // end of namespace tlib