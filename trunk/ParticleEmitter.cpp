#include "ParticleEmitter.h"
#include "Particle.h"
#include "Clock.h"
#include "Timer.h"

// ----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter():
m_bIsOn(false)
{
    init( 0.0, 1 );
}

ParticleEmitter::ParticleEmitter(
    double dReleaseTime, 
    int iReleaseCount ):
m_bIsOn(false)
{
    init( dReleaseTime, iReleaseCount );
}

// ----------------------------------------------------------------------------
void ParticleEmitter::start() 
{ 
    m_bIsOn = true;
    m_Timer->start();
}

// ----------------------------------------------------------------------------
void ParticleEmitter::init( double dReleaseTime, int iReleaseCount ) {
    // Request a timer from the application clock
    m_Timer = Clock::Instance().GetTimer();
    m_Timer->setDuration(dReleaseTime);
    m_iReleaseCount = iReleaseCount;
}