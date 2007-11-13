#include "QuatRotation.h"

namespace tlib
{

    /**
     * Multiplies the current rotation with another quaternion
     * and also returns the resulting rotation
     */
    const Quatf& OCQuatRotation::addRotation( const Quatf& qNewRot )
    {
        m_qRot = qNewRot * m_qRot;
        return m_qRot;
    }

    /**
     * Multiplies the current rotation with another quaternion
     * which creates internally and also returns the resulting rotation
     */
    const Quatf& OCQuatRotation::addRotation( const float fAngle, 
                                              const Vector3f& vAxis )
    {
        // Dont do anything for zero angle
        if( fabs(fAngle) < 1e-6f ) return m_qRot;

        Quatf qNewRot( fAngle, vAxis );
        qNewRot.toRotation();
        return addRotation( qNewRot );
    }

    // ----------------------------------------------------------------------------
    void OCQuatRotation::updateView( const Quatf& qRot ) {
        m_vView.selfRotate( qRot );
    }

    // ----------------------------------------------------------------------------
    void OCQuatRotation::updateUp( const Quatf& qRot ) {
        m_vUp.selfRotate( qRot );
    }

    // ----------------------------------------------------------------------------
    void OCQuatRotation::resetAngles()
    {
        m_qPitch.wxyz(1,0,0,0);
        m_qYaw.wxyz(1,0,0,0);
        m_qRoll.wxyz(1,0,0,0);
    }
    
    // ----------------------------------------------------------------------------
    const Quatf& OCQuatRotation::addPitch( const float fAngle )
    {
        // Dont do anything for zero angle
        if( fabs(fAngle) < 1e-6f ) return m_qRot;

        // Create pitch
        Quatf qPitch( fAngle, m_vRight );
        qPitch.toRotation();

        // Add pitch
        m_qPitch = qPitch * m_qPitch;

        return addRotation( qPitch );
    }

    // ----------------------------------------------------------------------------
    const Quatf& OCQuatRotation::addYaw( const float fAngle )
    {
        // Dont do anything for zero angle
        if( fabs(fAngle) < 1e-6f ) return m_qRot;

        // Create pitch
        Quatf qYaw( fAngle, m_vUp );
        qYaw.toRotation();

        // Add pitch
        m_qYaw = qYaw * m_qYaw;

        return addRotation( qYaw );
    }

    // ----------------------------------------------------------------------------
    const Quatf& OCQuatRotation::addRoll( const float fAngle )
    {
        // Dont do anything for zero angle
        if( fabs(fAngle) < 1e-6f ) return m_qRot;

        // Create pitch
        Quatf qRoll( fAngle, m_vView );
        qRoll.toRotation();

        // Add pitch
        m_qRoll = qRoll * m_qRoll;

        return addRotation( qRoll );
    }

    // ----------------------------------------------------------------------------
    void OCQuatRotation::update()
    {
        // Create new orientation
        Quatf qNewDir = m_qRot * getOwner()->getDir();

        // Update the object's direction quaternion
        getOwner()->setDir( qNewDir );

        m_qPrevRot = m_qRot;
        m_qRot.wxyz(1,0,0,0);
    }

} // end of namespace tlib
