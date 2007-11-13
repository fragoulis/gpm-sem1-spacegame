#include "CollisionBBox.h"

namespace tlib 
{
    OCCollisionBBox::OCCollisionBBox() {
        m_iType = BBox;
    }
    OCCollisionBBox::OCCollisionBBox( const Vector3f& vBBox ) {
        setBBox( vBBox );
        m_iType = BBox;
    }

} // end of namespace tlib