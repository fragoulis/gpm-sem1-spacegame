#include "CollisionBSphere.h"

namespace tlib 
{
    OCCollisionBSphere::OCCollisionBSphere() {
        m_iType = BSphere;
    }
    OCCollisionBSphere::OCCollisionBSphere( int iRadius ) {
        setRadius( iRadius );
    }

} // end of namespace tlib