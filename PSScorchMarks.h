#pragma once
#include "ParticleSystem.h"
#include "Color.h"

namespace tlib { class Object; }
using tlib::Object;

/**
 * This could be an component system object !?~!@*&^!@
 */
class PSScorchMarks : public ParticleSystem
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
    void init();

    /**
     * Creates a scorch mark in a given position
     */
    void onSpawn( Particle *particle );

};