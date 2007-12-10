#pragma once
#include "ParticleSystem.h"

namespace tlib { class Object; }
using tlib::Object;

/**
 * This could be an component system object !?~!@*&^!@
 */
class PSBigExplosion : public ParticleSystem
{
private:
    // The particle's limits
    float m_fSize[2], m_fVelocity[2];
    int m_iLifeSpan[2];
    float m_vfStartColor[4], m_vfColorFactor[4];

public:
    /**
     * Constructor
     */
    PSBigExplosion();

    /**
     * Updates all particle positions
     */
    void update();

    /**
     * Renders all particles
     */
    void render() const;

    /**
     * Setups a laser particle system
     */
    void init( const Vector3f &vSysPos );

    /**
     * Resets the position of a particle and gives a random direction and 
     * speed
     */
    void onSpawn( Particle *particle );

};