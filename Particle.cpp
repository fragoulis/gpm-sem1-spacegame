#include "Particle.h"
#include <ctime>

// This will save us some time
extern const double M_CLOCKS_PER_SEC;

Particle::Particle(): 
m_fEnergy(1.0f),
m_dLifeSpan(0.0)
{}

// ----------------------------------------------------------------------------
void Particle::start()
{
   //m_lInitTime = clock() - long(m_dLifeSpan * CLOCKS_PER_SEC);
   m_lInitTime = clock();
}

// ----------------------------------------------------------------------------
bool Particle::hasExpired() 
{
    // Particles with life span of zero never expire
    //if( m_dLifeSpan < 1e-6 ) return false;

    // Check pariticle's expiration time
    double dCurrent = (double)(clock() - m_lInitTime)*M_CLOCKS_PER_SEC;
    if( m_dLifeSpan < dCurrent ) {
        return true;
    }

    // 
    m_fEnergy = 1.0f - float(dCurrent / m_dLifeSpan);

    return false;
}

// ----------------------------------------------------------------------------
void Particle::bounce( const Vector3f &vColDir, float fSpeedRatio )
{
    m_vVelocity = 
        ( - 2.0f * m_vVelocity.dot( vColDir ) * vColDir + m_vVelocity ) * fSpeedRatio;
}