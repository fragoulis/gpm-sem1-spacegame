#include "LinearMovement.h"
#include "Object.h"
#include "Clock.h"

namespace tlib 
{
    OCLinearMovement::OCLinearMovement():  
      m_fVelocity(0.0f)
    {
        m_vDir = Vector3f::Front();
    }

    OCLinearMovement::OCLinearMovement( float fVel, const Vector3f& vDir ):
      m_fVelocity(fVel)
    {
        m_vDir = vDir;
    }

    void OCLinearMovement::update()
    {
        // Retrieve object's position
        const Vector3f& vPos = getOwner()->getPos();

        // Calculate new position
        const Vector3f vNewPos = 
            vPos + m_vDir * m_fVelocity * Clock::Instance().getDeltaTime();

        // Update position
        getOwner()->setPos( vNewPos );
    }

} // end of namespace tlib