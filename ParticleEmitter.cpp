#include "ParticleEmitter.h"
#include "Particle.h"
#include <ctime>

// This will save us some time
const double M_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

// ----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter():
m_bIsOn(false),
m_dReleaseTime(0.0),
m_iReleaseCount(1)
{}

ParticleEmitter::ParticleEmitter(
    double dReleaseTime, 
    int iReleaseCount ):
m_bIsOn(false),
m_dReleaseTime(dReleaseTime),
m_iReleaseCount(iReleaseCount)
{}

// ----------------------------------------------------------------------------
void ParticleEmitter::start() 
{ 
    m_bIsOn = true;
    m_lInitTime = clock() - long(m_dReleaseTime * CLOCKS_PER_SEC);
    //m_lInitTime = clock()
}

// ----------------------------------------------------------------------------
bool ParticleEmitter::checkRelease()
{
    // If release time is negligible just return true
    if( m_dReleaseTime < 1e-3 )
        return true;
    
    long curTime = clock();
    if( m_dReleaseTime < (double)(curTime - m_lInitTime)*M_CLOCKS_PER_SEC )
    {
        // If time passed, reset the initialization time to the current time
        m_lInitTime = curTime;
        return true;
    }
    
    return false;
}