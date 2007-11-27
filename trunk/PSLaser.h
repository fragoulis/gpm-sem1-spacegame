#pragma once
#include "PSCollidable.h"
#include "Color.h"

class PSScorchMarks;
namespace tlib { class Object; }
using tlib::Object;

/**
 * This could be an component system object !?~!@*&^!@
 */
class PSLaser : public PSCollidable
{
private:
    // The onwer object
    Object *m_oOwner;

    // The direction in which the emitter will spawn new particles
    // Used for caching the direcion vector before passing it to 
    // every new particle
    Vector3f m_vDir;

    // The system's offset to the owner object's position
    float m_fOffset;

    // The velocity of the laser bolts
    // Note: the velocity exists here and not in every particle
    // since in this system the particles are bound to have a constant
    // velocity from spawn to death.
    float m_fVelocity;

    // The color of the laser bolts
    Color m_Color;

    // An emmbeded particle system for the scortch marks
    PSScorchMarks *m_Marks;

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
    void init( Object *oOwner,
               const Vector3f &vSysPos, 
               float vEmitterOffset,
               float vfLaserColor[] );

private:
    void onCollisionWithTiles( Particle *particle, const Vector3f &vColDir );
    void onCollisionWithObjects( Particle *particle, const Vector3f &vColDir );

    /**
     * Resets the position of a particle and gives a random direction and 
     * speed
     */
    void onSpawn( Particle *particle );

    // empty
    void onKill( Particle *particle );
};