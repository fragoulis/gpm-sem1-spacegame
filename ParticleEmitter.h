#pragma once

/**
 * Abstract class for the particle emitters
 */
class ParticleEmitter
{
public:
    // The different types of emitters
    enum EmitterType {
        None = 0,
        Sphere,
        Cone,
        Line
    };

private:
    // The type of the emitter
    // Default: None
    int m_iType;

    // Flag for whether this emitter should spawn particles or not
    // Default: False
    bool m_bIsActive;

    // The interval between particle releases in seconds.
    // A release time of null means that the emitter will spawn particles
    // every frame
    // Default: 0.0
    double m_dReleaseTime;

    // The number of particles to be released/spawned every time.
    // Default: 1
    int m_iReleaseCount;

    // Holds the time passed between spawns
    long m_lInitTime;

public:
    /**
     * Constructors
     */
    ParticleEmitter();
    ParticleEmitter( int iType, 
                     bool bIsActive, 
                     double dReleaseTime, 
                     int iReleaseCount );

    /**
     * Destructor
     */
    virtual ~ParticleEmitter();

    /**
     * Starts the emitter
     */
    void start();

    /**
     * Stops the emitter
     */
    void stop() { m_bIsActive = false; }

    /**
     * Returns whether the emitter is active or not
     */
    bool isActive() const { return m_bIsActive; }

    /**
     * Getter/Setter for the emitter type
     */
    void setType( int iType ) { m_iType = iType; }
    //int getType() const { return m_iType; }

    /**
     * Spawns any number of particles while checking for the time
     * intervals
     */
    void spawn();

protected:
    /**
     * Spawns any number of particles
     */
    virtual void onSpawn() = 0;

private:
    /**
     * Checks the time intervals
     */
    bool _checkTime();

}; // end of ParticleEmitter class