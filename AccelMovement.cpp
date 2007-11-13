
#include "accelmovement.h"
#include "object.h"

namespace tlib 
{
    OCAccelMovement::OCAccelMovement():  
      m_fAccel(0.0f)
    {
        OCLinearMovement::OCLinearMovement();
    }

    OCAccelMovement::OCAccelMovement( float fAcc, 
                                      float fVel, 
                                      const Vector3f& vDir ):
      m_fAccel(fAcc)
    {
        OCLinearMovement::OCLinearMovement( fVel, vDir );
    }

    void OCAccelMovement::update()
    {
        // Retrieve object's position
        const Vector3f& vPos = getOwner()->getPos();

        // Rotate the direction vector
        //m_vDir.selfRotate( qRot );

        // Update the velocity if acceleration is greater than zero
        m_fVelocity += m_fAccel * IOCMovement::DeltaTime();

        const Vector3f 
            vVel = m_vDir * m_fVelocity,
            vAcc = m_vDir * m_fAccel;

        // Calculate new position
        Vector3f vNewPos = vPos + 
                           vVel * IOCMovement::DeltaTime() +
                           vAcc * IOCMovement::DeltaTime() * IOCMovement::DeltaTime() * 0.5f;

        // Update position
        getOwner()->setPos( vNewPos );
    }

} // end of namespace tlib