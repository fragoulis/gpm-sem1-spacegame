#include "AccelMovement.h"
#include "Object.h"
#include "Clock.h"

namespace tlib 
{
    OCAccelMovement::OCAccelMovement():  
    m_fAccel(0.0f)
    {}

    OCAccelMovement::OCAccelMovement( float fAcc, 
                                      float fVel, 
                                      const Vector3f& vDir ):
    OCLinearMovement( fVel, vDir ),
    m_fAccel(fAcc)
    {}

    // ----------------------------------------------------------------------------
    void OCAccelMovement::update()
    {
        // Retrieve object's position
        const Vector3f& vPos = getOwner()->getPos();
        const float dDeltaTime = Clock::Instance().getDeltaTime();
        // Update the velocity
        m_fVelocity += m_fAccel * dDeltaTime;

        const Vector3f 
            vVel = m_vDir * m_fVelocity,
            vAcc = m_vDir * m_fAccel;

        // Calculate new position
        const Vector3f vNewPos = vPos + 
                                 vVel * dDeltaTime +
                                 vAcc * dDeltaTime * dDeltaTime * 0.5f;

        // Update position
        getOwner()->setPos( vNewPos );
    }

} // end of namespace tlib