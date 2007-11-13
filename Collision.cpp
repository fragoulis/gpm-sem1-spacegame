//#include "Collision.h"
#include "CollisionBBox.h"
#include "CollisionBSphere.h"

namespace tlib 
{

    // ------------------------------------------------------------------------
    bool IOCCollision::check( Object *oObj, Vector3f& vCollDir ) const
    {
        // Get other object's collision component
        IComponent *cCom = oObj->getComponent("collision");
        IOCCollision *cObjCol = (IOCCollision*)cCom;
        if( !cObjCol )
            return false;

        // Switch between the differect kinds of collision
        if( m_iType == BBox && cObjCol->getType() == BBox ) {
            return checkBoxWithBox( 
                oObj->getPos(), 
                ((OCCollisionBBox*)cObjCol)->getBBox(),
                vCollDir );
        } 
        else if( m_iType == BBox && cObjCol->getType() == BSphere ) {
            return checkBoxWithSphere( 
                oObj->getPos(), 
                ((OCCollisionBSphere*)cObjCol)->getRadius(),
                vCollDir );
        } else {
            return checkSphereWithSphere( 
                oObj->getPos(), 
                ((OCCollisionBSphere*)cObjCol)->getRadius(),
                vCollDir );
        }

        // Programme will never go here
        return false;
    }

    // ------------------------------------------------------------------------
    bool IOCCollision::check( Object *oObj ) const
    {
        // Get other object's collision component
        IComponent *cCom = oObj->getComponent("collision");
        IOCCollision *cObjCol = (IOCCollision*)cCom;
        if( !cObjCol )
            return false;

        // Switch between the differect kinds of collision
        if( m_iType == BBox && cObjCol->getType() == BBox ) {
            return checkBoxWithBox( 
                oObj->getPos(), 
                ((OCCollisionBBox*)cObjCol)->getBBox() );
        } 
        else if( m_iType == BBox && cObjCol->getType() == BSphere ) {
            return checkBoxWithSphere( 
                oObj->getPos(), 
                ((OCCollisionBSphere*)cObjCol)->getRadius() );
        } else {
            return checkSphereWithSphere( 
                oObj->getPos(), 
                ((OCCollisionBSphere*)cObjCol)->getRadius() );
        }

        // Programme will never go here
        return false;
    }

    // ------------------------------------------------------------------------
    bool IOCCollision::checkBoxWithBox( 
        const Vector3f& vPos, 
        const Vector3f& vBBox ) const
    {
        // Sace the difference of the objects' positions
        const Vector3f& vDiffPos = getOwner()->getPos() - vPos;
        // Save the sum of the boxes' halfsizes
        const Vector3f vHalf = vBBox + ((OCCollisionBBox*)this)->getBBox();

        // Check x-axiz
        if( fabs(vDiffPos.x()) > vHalf.x() ) return false;
        // Check y-axiz
        if( fabs(vDiffPos.y()) > vHalf.y() ) return false;
        // Check z-axiz
        if( fabs(vDiffPos.z()) > vHalf.z() ) return false;

        return true;
    }

    // ------------------------------------------------------------------------
    bool IOCCollision::checkBoxWithSphere( 
        const Vector3f& vPos, 
        int iRadius ) const
    {
        return 1;

    } // enf checkBoxWithSphere()

    // ------------------------------------------------------------------------
    bool IOCCollision::checkSphereWithSphere( 
        const Vector3f& vPos, 
        int iRadius ) const
    {
        return 1;

    } // enf checkSphereWithSphere()

    // ------------------------------------------------------------------------
    bool IOCCollision::checkBoxWithBox( 
        const Vector3f& vPos, 
        const Vector3f& vBBox,
        Vector3f& vCollDir ) const
    {
        // Sace the difference of the objects' positions
        const Vector3f& vDiffPos = getOwner()->getPos() - vPos;
        // Save the sum of the boxes' halfsizes
        const Vector3f vHalf = vBBox + ((OCCollisionBBox*)this)->getBBox();

        // Check x-axis overlap
        const float fOverlapX = vHalf.x() - fabs(vDiffPos.x());
        if( fOverlapX > 0.0f ) 
        {
            // Check y-axis overlap
            const float fOverlapY = vHalf.y() - fabs(vDiffPos.y());
            if( fOverlapY > 0.0f ) 
            {
                // Check z-axis overlap
                const float fOverlapZ = vHalf.z() - fabs(vDiffPos.z());
                if( fOverlapZ > 0.0f ) 
                {
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
                            vCollDir.xyz( fOverlapX, 0.0f, 0.0f );
                        } else { 
                            // Object came from left
                            vCollDir.xyz( -fOverlapX, 0.0f, 0.0f );
                        }
                    }
                    else if( fOverlapY < fOverlapX && fOverlapY < fOverlapZ ) {
                        // The smallest overlap is at y-axis
                        if( vDiffPos.y() > 0.0f ) { 
                            // Object came from up
                            vCollDir.xyz( 0.0f, fOverlapY, 0.0f );
                        } else { 
                            // Object came from down
                            vCollDir.xyz( 0.0f, -fOverlapY, 0.0f );
                        }
                    }
                    else if( fOverlapZ < fOverlapX && fOverlapZ < fOverlapY ) {
                        // The smallest overlap is at z-axis
                        if( vDiffPos.z() > 0.0f ) { 
                            // Object came from back
                            vCollDir.xyz( 0.0f, 0.0f, fOverlapZ );
                        } else { 
                            // Object came from front
                            vCollDir.xyz( 0.0f, 0.0f, -fOverlapZ );
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
    bool IOCCollision::checkBoxWithSphere( 
        const Vector3f& vPos, 
        int iRadius,
        Vector3f& vCollDir ) const
    {
        return 1;

    } // enf checkBoxWithSphere()

    // ------------------------------------------------------------------------
    bool IOCCollision::checkSphereWithSphere( 
        const Vector3f& vPos, 
        int iRadius,
        Vector3f& vCollDir ) const
    {
        // Sace the difference of the objects' positions
        const Vector3f& vDiffPos = getOwner()->getPos() - vPos;
        // Save the sum of the spheres' radius
        const int iSumRad = iRadius + ((OCCollisionBSphere*)this)->getRadius();

         // Check x-axis overlap
        const float fOverlapX = iSumRad - fabs(vDiffPos.x());
        if( fOverlapX > 0.0f ) 
        {
            // Check y-axis overlap
            const float fOverlapY = iSumRad - fabs(vDiffPos.y());
            if( fOverlapY > 0.0f ) 
            {
                // Check z-axis overlap
                const float fOverlapZ = iSumRad - fabs(vDiffPos.z());
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
                    vCollDir.mul( fOverlapX, fOverlapY, fOverlapZ );

                    // Collision detected
                    return true;

                } // end if( )
            } // end if( )
        } // end if( )

        // No collision detected
        return false;

    } // end checkSphereWithSphere()

} // end of namespace tlib