#include "ParticleEmitter.h"
#include <ctime>

// This will save us some time
const double M_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

// ----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter():
m_iType(None),
m_bIsActive(false),
m_dReleaseTime(0.0),
m_iReleaseCount(1)
{}

ParticleEmitter::ParticleEmitter( 
    int iType,
    bool bIsActive, 
    double dReleaseTime, 
    int iReleaseCount ):
m_iType(iType),
m_bIsActive(bIsActive),
m_dReleaseTime(dReleaseTime),
m_iReleaseCount(iReleaseCount)
{
    // If active flag is true, start immediately
    if( bIsActive )
        start();
}

ParticleEmitter::~ParticleEmitter() 
{}

// ----------------------------------------------------------------------------
void ParticleEmitter::start() 
{ 
    m_bIsActive = true; 
    m_lInitTime = clock();
}

// ----------------------------------------------------------------------------
bool ParticleEmitter::_checkTime()
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

// ----------------------------------------------------------------------------
void ParticleEmitter::spawn()
{
    // If time is not right don't do anything
    if( !_checkTime() ) return;

    // Call spawn from the derived class
    onSpawn();
}