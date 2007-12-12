#pragma once
#include "ParticleSystem.h"

/**
 * This could be an component system object !?~!@*&^!@
 */
class PSTemplate;
class PSCommon : public ParticleSystem
{
    friend PSTemplate;
private:
        // The particle system's attributes
    float 
        m_fSize[2], 
        m_fVelocity[2],
        m_vfColor[4], 
        m_vfColorFactor[4],
        m_vfDirection[3][2];
    double 
        m_dReleaseTime,
        m_dSysDuration;
    int 
        m_iReleaseCount,
        m_iLifeSpan[2],
        m_iNumOfParticles;

public:
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
    void init( const PSTemplate &Template, const Vector3f &vSysPos );

    /**
     * Resets the position of a particle and gives a random direction and 
     * speed
     */
    virtual void onSpawn( Particle *particle );

};