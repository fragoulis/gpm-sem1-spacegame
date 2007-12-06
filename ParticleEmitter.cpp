#include "ParticleEmitter.h"
#include "Particle.h"
#include <ctime>

// ----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter():
m_bIsOn(false),
m_iReleaseCount(1)
{}

ParticleEmitter::ParticleEmitter(
    double dReleaseTime, 
    int iReleaseCount ):
m_bIsOn(false),
m_iReleaseCount(iReleaseCount)
{
    m_Timer.setDuration(dReleaseTime);
}

// ----------------------------------------------------------------------------
void ParticleEmitter::start() 
{ 
    m_bIsOn = true;
    m_Timer.start();
}