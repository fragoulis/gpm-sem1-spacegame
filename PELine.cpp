#include "PELine.h"
#include "Particle.h"

PELine::PELine(): 
ParticleEmitter()
{
    setType( Line );
}

PELine::PELine( 
    bool bIsActive, 
    double dReleaseTime, 
    int iReleaseCount ):
ParticleEmitter( Line, bIsActive, dReleaseTime, iReleaseCount )
{}

// ----------------------------------------------------------------------------
void PELine::onSpawn( const Vector3f &vSysPos, Particle *particle ) 
{
    particle->setPos( vSysPos );
    particle->setDir( m_vDir );
}

void PELine::onKill( Particle *particle )
{}