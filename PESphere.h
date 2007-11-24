#pragma once
#include "ParticleEmitter.h"

/**
 * PESphere stands for particle emitter of type sphere.
 * It creates particles in all directions starting from the same location
 */
class PESphere : public ParticleEmitter
{
private:
    // The imaginary's sphere radius
    // Default: 1.0f
    float m_fRadius;

public:
    /**
     * Constructor
     */
    PESphere();
    PESphere( float fRadius, 
              bool bIsActive, 
              double dReleaseTime, 
              int iReleaseCount );

    /**
     * Spawns any number of particles
     */
    void onSpawn();

}; // end of PESphere class