#include "CollisionResponse.h"
#include "Object.h"

namespace tlib
{
    void IOCCollisionResponse::respond()
    {
        m_oOwner->setPos( m_oOwner->getPos() + m_vCollDir );
    }

} // end of namespace tlib