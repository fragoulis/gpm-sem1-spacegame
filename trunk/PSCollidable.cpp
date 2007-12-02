#include "PSCollidable.h"
#include "Particle.h"
#include "ObjectMgr.h"
#include "Tilemap.h"
#include "CollisionBBox.h"
#include "CollisionBSphere.h"
using namespace tlib;

PSCollidable::PSCollidable():
m_CurTile(0),
m_PrevTile(0)
{}

PSCollidable::~PSCollidable()
{}

// ----------------------------------------------------------------------------
void PSCollidable::init() {
    m_iTileSize = Tilemap::Instance().getTileSize();
    m_iHalfNumOfTiles = (int)(Tilemap::Instance().getNumOfTiles() * 0.5f);
}

// ----------------------------------------------------------------------------
bool PSCollidable::readTile( Particle *particle )
{   
    // Save the tile in which the object is
    m_CurTile = Tilemap::Instance().getTile( particle->getPos() );
       
    return (0==m_CurTile)?0:1;
}

// ----------------------------------------------------------------------------
bool PSCollidable::readPrevTile( Particle *particle )
{
    // Save the tile in which the object was
    m_PrevTile = Tilemap::Instance().getTile( particle->getPrevPos() );
      
    return (0==m_PrevTile)?0:1;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void PSCollidable::checkCollision( Particle *particle )
{
    // The collision direction
    Vector3f vColDir;
    // The object to check against
    Object *objToCheck;

    // ________________________________________________________________________
    // If particle is currently inside a corridor
    if( readTile( particle ) )
    {
        // Check it against the tile's occupier
        objToCheck = getTile()->getOccupant();

        // Check for collisions with tile occupier
        if( objToCheck )
        {
            if( checkCollisionWithObject( particle, 
                                          objToCheck, 
                                          vColDir ) ) 
            {
                onCollisionWithObjects( particle, vColDir, objToCheck );
                return;
            }
        }
    }
    // ________________________________________________________________________
    // If particle is just outside a corridor
    else if( readPrevTile( particle ) )
    {
        Vector3f vColPoint;

        // Check the collision with the corridor walls
        if( checkCollisionWithTiles( particle->getPos(), 
                                     vColDir,
                                     vColPoint ) ) 
        {   
            onCollisionWithTiles( particle, vColDir, vColPoint );
            return;
        }
    }

    // ________________________________________________________________________
    // Before checking with the spaceship or the shield check if this particle
    // has an owner with type of spaceship
    // If so, skip the next check
    if( m_oOwner && m_oOwner->getType() != Object::SPACESHIP )
    {
        // Check particle against the spaceship's shield, if shield is active
        objToCheck = (Object*)&ObjectMgr::Instance().getShield();
        if( !objToCheck->isActive() ) {
            // If shield is not active, it means that the ship has been 
            // massively damaged.
            // So, instead, check with the ship
            objToCheck = (Object*)&ObjectMgr::Instance().getShip();
        }
        
        // Check either with ship or shield
        if( checkCollisionWithObject( particle, objToCheck, vColDir ) )
        {   
            onCollisionWithObjects( particle, vColDir, objToCheck );
            return;
        }
    }

    // ________________________________________________________________________
    // The final check consists of checking against the reactor if everything 
    // else did not hit
    objToCheck = (Object*)&ObjectMgr::Instance().getReactor();
    if( checkCollisionWithObject( particle, 
                                  objToCheck, 
                                  vColDir ) )
    {   
        onCollisionWithObjects( particle, vColDir, objToCheck );
    }

} // end checkCollision()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool PSCollidable::checkCollisionWithTiles( const Vector3f &vPos,
                                            Vector3f &vCollDir,
                                            Vector3f &vCollPoint )
{
    _ASSERT(m_PrevTile!=0);

    // Here we check for collision with each of the tile's planes
    if( m_PrevTile->getType() & TW_LEFT ) {
        // Check collision with the left plane
        int x = ( m_PrevTile->i() - m_iHalfNumOfTiles ) * m_iTileSize;
        if( vPos.x() < (float)x ) {
            vCollDir.xyz( 1.0f, 0.0f, 0.0f );
            vCollPoint.xyz( (float)x, vPos.y(), vPos.z() );
            //std::cout << "Left collision" << std::endl;
            return true;
        }
    }
    
    if( m_PrevTile->getType() & TW_RIGHT ) {
        // Check collision with the right plane
        int x = ( ( m_PrevTile->i() + 1 ) - m_iHalfNumOfTiles ) * m_iTileSize;
        if( (float)x < vPos.x() ) {
            vCollDir.xyz( -1.0f, 0.0f, 0.0f );
            vCollPoint.xyz( (float)x, vPos.y(), vPos.z() );
            //std::cout << "Right collision" << std::endl;
            return true;
        }
    }

    if( m_PrevTile->getType() & TW_BOTTOM ) {
        // Check collision with the bottom plane
        int y = ( m_PrevTile->j() - m_iHalfNumOfTiles ) * m_iTileSize;
        if( vPos.y() < (float)y ) {
            vCollDir.xyz( 0.0f, 1.0f, 0.0f );
            vCollPoint.xyz( vPos.x(), (float)y, vPos.z() );
            //std::cout << "Bottom collision" << std::endl;
            return true;
        }
    }
    
    if( m_PrevTile->getType() & TW_TOP ) {
        // Check collision with the top plane
        int y = ( ( m_PrevTile->j() + 1 ) - m_iHalfNumOfTiles ) * m_iTileSize;
        if( (float)y < vPos.y() ) {
            vCollDir.xyz( 0.0f, -1.0f, 0.0f );
            vCollPoint.xyz( vPos.x(), (float)y, vPos.z() );
            //std::cout << "Top collision" << std::endl;
            return true;
        }
    }
    
    if( m_PrevTile->getType() & TW_BACK ) {
        // Check collision with the back plane
        int z = ( m_iHalfNumOfTiles - m_PrevTile->k() ) * m_iTileSize;
        if( (float)z < vPos.z() ) {
            vCollDir.xyz( 0.0f, 0.0f, -1.0f );
            vCollPoint.xyz( vPos.x(), vPos.y(), (float)z );
            //std::cout << "Back collision" << std::endl;
            return true;
        } 
    }

    if( m_PrevTile->getType() & TW_FRONT ) {
        // Check collision with the front plane
        int z = ( m_iHalfNumOfTiles - ( m_PrevTile->k() + 1 ) ) * m_iTileSize;
        if( vPos.z() < (float)z ) {
            vCollDir.xyz( 0.0f, 0.0f, 1.0f );
            vCollPoint.xyz( vPos.x(), vPos.y(), (float)z );
            //std::cout << "Front collision" << std::endl;
            return true;
        }
    }

    return false;

} // end checkCollisionWithTiles()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool PSCollidable::checkCollisionWithObject( Particle *particle,
                                             Object *oObj,
                                             Vector3f &vCollDir )
{
    // Get object's collision component
    IComponent *cCom = oObj->getComponent("collision");
    IOCCollision *cObjCol = (IOCCollision*)cCom;
    _ASSERT(cObjCol!=0);
    // If object has is not collidable return false
    //if( !cObjCol )
    //    return false;

    // Switch between the differect kinds of collision
    if( cObjCol->getType() == IOCCollision::BBox ) {
        return checkWithBox(
            particle, 
            oObj->getPos(), 
            ((OCCollisionBBox*)cObjCol)->getBBox(),
            vCollDir );
    } 
    else if( cObjCol->getType() == IOCCollision::BSphere ) {
        return checkWithSphere( 
            particle, 
            oObj->getPos(), 
            ((OCCollisionBSphere*)cObjCol)->getRadius(),
            vCollDir );
    }

    return false;
}

// ------------------------------------------------------------------------
bool PSCollidable::checkWithBox( 
    Particle *particle,
    const Vector3f& vPos, 
    const Vector3f& vBBox,
    Vector3f& vCollDir ) const
{
    // Sace the difference of the objects' positions
    const Vector3f& vDiffPos = particle->getPos() - vPos;
    // Save the sum of the boxes' halfsizes
    const Vector3f& vHalf = vBBox + particle->getSize();

    // Check x-axis overlap
    const float fOverlapX = vHalf.x() - fabs(vDiffPos.x());
    if( fOverlapX > 0.0f ) 
    {
        //std::cout << "Overlap X " << std::endl;
        // Check y-axis overlap
        const float fOverlapY = vHalf.y() - fabs(vDiffPos.y());
        if( fOverlapY > 0.0f ) 
        {
            //std::cout << "Overlap Y " << std::endl;
            // Check z-axis overlap
            const float fOverlapZ = vHalf.z() - fabs(vDiffPos.z());
            if( fOverlapZ > 0.0f ) 
            {
                //std::cout << "Overlap Z " << std::endl;
                // If we reach this point the two objects
                // overlap in all three axis and hence we have
                // collision
                // No we try to find the smallest overlap of the three
                // and then we will return the collision direction
                // along with the amount of penetration
                if( fOverlapX < fOverlapY && fOverlapX < fOverlapZ ) {
                    // The smallest overlap is at x-axis
                    if( vDiffPos.x() > 0.0f ) { 
                        // Object came from right
                        //std::cout << "Right collision" << std::endl;
                        vCollDir.xyz( 1.0f, 0.0f, 0.0f );
                    } else { 
                        // Object came from left
                        //std::cout << "Left collision" << std::endl;
                        vCollDir.xyz( -1.0f, 0.0f, 0.0f );
                    }
                }
                else if( fOverlapY < fOverlapX && fOverlapY < fOverlapZ ) {
                    // The smallest overlap is at y-axis
                    if( vDiffPos.y() > 0.0f ) { 
                        // Object came from up
                        //std::cout << "Top collision" << std::endl;
                        vCollDir.xyz( 0.0f, 1.0f, 0.0f );
                    } else { 
                        // Object came from down
                        //std::cout << "Bottom collision" << std::endl;
                        vCollDir.xyz( 0.0f, -1.0f, 0.0f );
                    }
                }
                else if( fOverlapZ < fOverlapX && fOverlapZ < fOverlapY ) {
                    // The smallest overlap is at z-axis
                    if( vDiffPos.z() > 0.0f ) { 
                        // Object came from back
                        //std::cout << "Front collision" << std::endl;
                        vCollDir.xyz( 0.0f, 0.0f, 1.0f );
                    } else { 
                        // Object came from front
                        //std::cout << "Back collision" << std::endl;
                        vCollDir.xyz( 0.0f, 0.0f, -1.0f );
                    }
                }

                // Collision detected
                return true;

            } // end if( )
        } // end if( )
    } // end if( )

    // No collision detected
    return false;

} // enf checkBoxWithBox()

// ------------------------------------------------------------------------
bool PSCollidable::checkWithSphere( 
    Particle *particle,
    const Vector3f& vPos, 
    float fRadius,
    Vector3f& vCollDir ) const
{
    // Sace the difference of the objects' positions
    const Vector3f& vDiffPos = particle->getPos() - vPos;
    // Save the sum of the spheres' radius
    const float fSumRad = fRadius + particle->getSize();

     // Check x-axis overlap
    const float fOverlapX = fSumRad - fabs(vDiffPos.x());
    if( fOverlapX > 0.0f ) 
    {
        // Check y-axis overlap
        const float fOverlapY = fSumRad - fabs(vDiffPos.y());
        if( fOverlapY > 0.0f ) 
        {
            // Check z-axis overlap
            const float fOverlapZ = fSumRad - fabs(vDiffPos.z());
            if( fOverlapZ > 0.0f ) 
            {
                // If we reach this point the two objects
                // overlap in all three axis and hence we have
                // collision
                // The position difference indicates the collision 
                // direction so we save the normalized version to the
                // collision vector
                vDiffPos.normalize( vCollDir );
                // Then we mulitply with the overlap components to 
                // return not only the collision direction but the 
                // amount of penetration as well
                //vCollDir.mul( fOverlapX, fOverlapY, fOverlapZ );
                //std::cout << vCollDir << std::endl;

                // Collision detected
                return true;

            } // end if( )
        } // end if( )
    } // end if( )

    // No collision detected
    return false;

} // end checkSphereWithSphere()