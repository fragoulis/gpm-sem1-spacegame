#include "Particle.h"
#include <ctime>

// ----------------------------------------------------------------------------
void Particle::bounce( const Vector3f &vColDir, float fSpeedRatio )
{
    m_vVelocity = 
        ( - 2.0f * m_vVelocity.dot( vColDir ) * vColDir + m_vVelocity ) * fSpeedRatio;

    //// Normalize velocity
    //m_vVelocity.normalize();
    //// Calculate bounce vector
    //Vector3f vBounce = ( - 2.0f * m_vVelocity.dot( vColDir ) * vColDir + m_vVelocity );
    //vBounce.normalize();
    //// Set the new velocity vector
    //m_vVelocity = vBounce * m_fVelocity * fSpeedRatio;
}