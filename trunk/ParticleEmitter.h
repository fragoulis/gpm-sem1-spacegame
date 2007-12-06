#pragma once
#include <list>
using namespace std;

#include "Vector3.h"
#include "Timer.h"
using tlib::Vector3f;
using tlib::Timer;

class Particle;

// Particle list typedef
typedef list<Particle*> ParticleList;

/**
 * Abstract class for the particle emitters
 */
class ParticleEmitter
{
protected:
    // The number of particles to be released/spawned every time.
    // Default: 1
    int m_iReleaseCount;

private:
    // Flag for whether this emitter should spawn particles or not
    // Default: False
    bool m_bIsOn;

    // The array of alive[updatable] particles
    ParticleList m_vPAlive;

    // The array of dead particles
    ParticleList m_vPDead;

    // Particle emitter's timer
    Timer m_Timer;

public:
    /**
     * Constructors
     */
    ParticleEmitter();
    ParticleEmitter( double dReleaseTime, int iReleaseCount );

    /**
     * Starts the emitter
     */
    void start();

    /**
     * Stops the emitter
     */
    void stop() { m_bIsOn = false; }

    /**
     * Returns whether the emitter is active or not
     */
    bool isOn() const { return m_bIsOn; }

    /**
     * Timer accessor
     */
    const Timer& getTimer() const { return m_Timer; }
    Timer& getTimer() { return m_Timer; }

    /**
     * Initializes the emitter
     */
    void init( double dReleaseTime, int iReleaseCount ) {
        m_Timer.setDuration(dReleaseTime);
        m_iReleaseCount = iReleaseCount;
    }

    /**
     * Release count accessor
     */
    int getReleaseCount() const { return m_iReleaseCount; }

    /**
     * Getters for the particle lists
     */
    const ParticleList& getPAlive() const {
        return m_vPAlive;
    }
    ParticleList& getPAlive() {
        return m_vPAlive;
    }
    const ParticleList& getPDead() const {
        return m_vPDead;
    }
    ParticleList& getPDead() {
        return m_vPDead;
    }

}; // end of ParticleEmitter class