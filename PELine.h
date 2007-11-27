#pragma once
#include "ParticleEmitter.h"

class Particle;

/**
 * The line emitter is used by the laser particle systems
 * to spawn particles on a certain direction
 */
class PELine : public ParticleEmitter
{
private:
    // The direction in which the emitter will spawn new particles
    Vector3f m_vDir;

public:
    /**
     * Constructor
     */
    PELine();
    PELine( bool bIsActive, 
            double dReleaseTime, 
            int iReleaseCount );

    /**
     * Direction setter
     */
    void setDir( const Vector3f &vDir ) {
        m_vDir = vDir;
    }

    /**
     * Resets the position of a particle and gives a random direction and 
     * speed
     */
    void onSpawn( const Vector3f &vSysPos, Particle *particle );

    // empty
    void onKill( Particle *particle );

}; // end of PESphere class