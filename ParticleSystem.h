#pragma once
#include "ParticleEmitter.h"
#include <string>

namespace tlib { class Object; }
using tlib::Object;

class ParticleSystem
{
protected:
    // The particle system's position
    Vector3f m_vPos;

    // The physical array of particles
    Particle *m_Particles;

    // The particle system's emitter
    ParticleEmitter m_Emitter;

    // The opengl's texture id
    // This is only used by systems with textures
    unsigned int m_uiTexId;

    // The particle's display list id
    unsigned int m_uiListId;

    // Every particle system can have an owner object,
    // most likely the one its emitting from
    Object *m_oOwner;

    // The particle system's timer
    Timer *m_Timer;

public:
    /**
     * Constructor
     */
    ParticleSystem();

    /**
     * Destructor
     */
    virtual ~ParticleSystem();

    /**
     * Particle emitter accessor
     */
    const ParticleEmitter& getEmitter() const { 
        return m_Emitter; 
    }
    ParticleEmitter& getEmitter() { 
        return m_Emitter; 
    }

    /**
     * Timer accessor
     */
    Timer* getTimer() { return m_Timer; }

    /**
     * Setter for the position
     */
    void setPos( const Vector3f &vPos ) {
        m_vPos = vPos;
    }

    /**
     * Starts the particle system
     */
    void start();

    /**
     * Spawns a iRealaseCount number of particles
     */
    void spawn();

    /**
     * Puts a particle array to the dead list
     */
    void kill( ParticleList &toKill );

    /**
     * Initializes the particle system
     */
    void init( float fParticleSize );

    /**
     * Updates all child particles
     */
    virtual void update() = 0;

    /**
     * Renders all particles
     */
    virtual void render() const = 0;

protected:
    /**
     * Assigns a texture to the particles
     */
    void setTexture( const std::string sTexture );

    /**
     * Is called for every particle that is created
     */
    virtual void onSpawn( Particle *particle ) = 0;

}; // end of ParticleSystem class