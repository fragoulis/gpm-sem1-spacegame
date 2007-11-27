#pragma once
#include "ParticleEmitter.h"

class Particle;

/**
 * PERandom emitter is used by the laser system to spawn
 * particles for scorch marks in random positions and directions
 */
class PERandom : public ParticleEmitter
{
private:
    // The direction in which the emitter will spawn new particles
    Vector3f m_vDir;

public:
    /**
     * Direction setter
     */
    void setDir( const Vector3f &vDir ) {
        m_vDir = vDir;
    }

    

}; // end of PERandom class