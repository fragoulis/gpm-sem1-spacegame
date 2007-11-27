#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Movement.h"

using tlib::Quati;
using tlib::Vector3f;
using tlib::IOCMovement;

/**
 * Abstract class for the particles
 */
class Particle
{
private:
    // Pointers to the previous and the next particle
    Particle *m_pPrev, *m_pNext;

    // The particle's position
    Vector3f m_vPos;
    Vector3f m_vPrevPos;

    // The particle's direction
    Vector3f m_vDir;

    // Particle's velocity
    Vector3f m_vVelocity;

    // The particle's rotation
    // [Can be replaced by a 4d vector]
    Quati m_qRot;

    // The size of the particle
    float m_fSize;

    // The particle's life is seconds
    // Default: 0.0 [never dies]
    double m_dLifeSpan;
    // Holds the initialization time
    long m_lInitTime;

    // Particle's Energy
    // Scope: [0.0f, 1.0f]
    // Default: 1.0f
    float m_fEnergy;

public:
    /**
     * Constructor
     */
    Particle();

    /**
     * Getter/Setter for the position
     */
    const Vector3f& getPos() const { 
        return m_vPos; 
    }

    void setPos( const Vector3f &vPos ) { 
        m_vPos = vPos; 
    }

    const Vector3f& getPrevPos() const { 
        return m_vPrevPos; 
    }

    const Vector3f& getDir() const {
        return m_vDir;
    }

    void setDir( const Vector3f &vDir ) {
        m_vDir = vDir;
    }
    void setVelocity( const Vector3f &vVelocity ) {
        m_vVelocity = vVelocity;
    }

    void updatePos() { 
        m_vPrevPos = m_vPos;
        m_vPos += m_vVelocity * IOCMovement::DeltaTime();
    }

    void setSize( float fSize ) { m_fSize = fSize; }
    float getSize() const { return m_fSize; }

    void setLifeSpan( double dLifeSpan ) { 
        m_dLifeSpan = dLifeSpan; 
    }
    double getLifeSpan() const { return m_dLifeSpan; }

    void setInitTime( long lInitTime ) {
        m_lInitTime = lInitTime;
    }

    long getInitTime() const { return m_lInitTime; }

    float getEnergy() const { return m_fEnergy; }
    
    void setVelocity( float fVelocity ) { 
        m_vVelocity = m_vDir * fVelocity;
    }

    void setRot( const Quati &qRot ) {
        m_qRot = qRot;
    }
    const Quati& getRot() const { return m_qRot; }

    void setPrev( Particle *particle ) {
        m_pPrev = particle;
    }
    Particle* getPrev() { return m_pPrev; }

    void setNext( Particle *particle ) {
        m_pNext = particle;
    }
    Particle* getNext() { return m_pNext; }

    bool hasExpired();

};