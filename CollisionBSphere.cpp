#include "CollisionBSphere.h"

namespace tlib 
{
    OCCollisionBSphere::OCCollisionBSphere() {
        m_iType = BSphere;
    }
    OCCollisionBSphere::OCCollisionBSphere( float fRadius ) {
        setRadius( fRadius );
        m_iType = BSphere;
    }

} // end of namespace tlib