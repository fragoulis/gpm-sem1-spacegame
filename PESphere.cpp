#include "PESphere.h"

PESphere::PESphere(): 
ParticleEmitter(),
m_fRadius(1.0f)
{
    setType( Sphere );
}

PESphere::PESphere( 
    float fRadius,
    bool bIsActive, 
    double dReleaseTime, 
    int iReleaseCount ):
ParticleEmitter( Sphere, bIsActive, dReleaseTime, iReleaseCount ),
m_fRadius(fRadius)
{}

// ----------------------------------------------------------------------------
void PESphere::onSpawn() 
{
    
}