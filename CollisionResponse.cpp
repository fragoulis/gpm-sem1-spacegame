#include "CollisionResponse.h"
#include "Object.h"

namespace tlib
{
    void IOCCollisionResponse::respond( const Vector3f &vCollVector ) const
    {
        m_oOwner->setPos( m_oOwner->getPos() + vCollVector );
    }

} // end of namespace tlib