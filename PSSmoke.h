#pragma once
#include "ParticleSystem.h"
#include "Color.h"

namespace tlib { class Object; }
using tlib::Object;

/**
 * This could be an component system object !?~!@*&^!@
 */
class PSSmoke : public ParticleSystem
{
private:
    // The particle's limits
    float m_fSize[2], m_fLifeSpan[2], m_fVelocity[2];

    // The color of the smoke particles
    Color m_Color;

public:
    /**
     * Constructor
     */
    PSSmoke();

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

}; // end of PSSmoke class