#pragma once
#include "ParticleEmitter.h"
#include <string>

namespace tlib { class Object; }
using tlib::Object;

class ParticleSystem
{
public:
    enum PSType {
        Any = 0,
        SmallExplosion,
        Smoke
    };

protected:
    // The particle system's position
    Vector3f m_vPos;

    // The physical array of particles
    Particle *m_Particles;

    // The particle system's emitter
    ParticleEmitter m_Emitter;

    // This shows for how long should a particle system emit for, before
    // it becomes inactive
    // Default: 0.0 [Never stops]
    double m_dLifeSpan;

    // The opengl's texture id
    // This is only used by systems with textures
    unsigned int m_uiTexId;

    // The particle's display list id
    unsigned int m_uiListId;

    // An array of random numbers
    static float m_vfRandom[500];

    // Every particle system can have an owner object,
    // most likely the one its emitting from
    Object *m_oOwner;

private:
    // Holds the initialization time
    long m_lInitTime;

    // Flags whether the particle system has expired or not
    bool m_bIsExpired;

    // The particle system's type
    // Default: 0[Any]
    PSType m_iType;

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
     * Getter for the emitter object
     */
    const ParticleEmitter& getEmitter() const { 
        return m_Emitter; 
    }
    ParticleEmitter& getEmitter() { 
        return m_Emitter; 
    }

    /**
     * Return whether the particle system has expired or not
     */
    bool isExpired() const { return m_bIsExpired; }

    /**
     * Returns true if the particle system is of the same type 
     * as the given parameter
     */
    bool isType( int iType ) const { return (m_iType==iType); }

    /**
     * Setter for the position
     */
    void setPos( const Vector3f &vPos ) {
        m_vPos = vPos;
    }

    /**
     * Sets the particle system's type
     */
    void setType( PSType iType ) {
        m_iType = iType;
    }

    /**
     * Starts the particle system
     */
    void start();

    /**
     * Check whether the particle has expired, therefore is not to be used
     */
    bool hasExpired();

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

private:
    /**
     * Generates an array of random numbers to be used
     * for the particle spawning
     */
    void genNumbers();

}; // end of ParticleSystem class