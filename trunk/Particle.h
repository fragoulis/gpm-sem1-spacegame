#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Clock.h"

using tlib::Quati;
using tlib::Vector3f;

/**
 * Abstract class for the particles
 */
class Particle
{
private:
    // Pointers to the previous and the next particle
    //Particle *m_pPrev, *m_pNext;

    // The particle's position
    Vector3f m_vPos;
    Vector3f m_vPrevPos;

    // Particle's velocity
    float m_fVelocity;
    Vector3f m_vVelocity;

    // The particle's rotation
    // [Can be replaced by a 4d vector]
    Quati m_qRot;

    // The size of the particle
    float m_fSize;

    // Particle's life
    int m_iLife;
    int m_iStartLife;

public:
    /**
     * Getter/Setter for the position
     */
    const Vector3f& getPos() const { return m_vPos; }
    void setPos( const Vector3f &vPos ) { 
        m_vPos = vPos; 
    }

    /**
     * Previous position accessor
     */
    const Vector3f& getPrevPos() const { 
        return m_vPrevPos; 
    }

    /**
     * Changes the direction for the particle
     */
    void setDir( const Vector3f &vDir ) {
        m_vVelocity = vDir * m_fVelocity;
    }

    /**
     * Sets the scalar velocity of the particle
     */
    void setVelocity( float fVelocity ) {
        m_fVelocity = fVelocity;
    }

    /**
     * Sets the direction velocity of the particle
     */
    void setVelocity( const Vector3f &vVel ) {
        m_vVelocity = vVel;
    }
    /**
     * Directional velocity accessor
     */
    const Vector3f& getVelocity() const {
        return m_vVelocity;
    }

    /**
     * Update the particle's position and energy
     */
    void update() { 
        // Save the current position
        m_vPrevPos = m_vPos;
        // Find new position
        m_vPos += m_vVelocity * Clock::Instance().getDeltaTime();
    }

    /**
     * Size getter/setter
     */
    void setSize( float fSize ) { m_fSize = fSize; }
    float getSize() const { return m_fSize; }

    /**
     * Particle life getter/setter
     */
    void updateLife() { m_iLife--; }
    int getLife() const { return m_iLife; }
    void setLife( int iLife ) {
        m_iLife = iLife;
    }
    void setStartLife( int iStartLife ) {
        m_iStartLife = iStartLife;
        m_iLife = iStartLife;
    }
    float getLifeRatio() const { 
        return (float)m_iLife / m_iStartLife;
    }
    void resetLife() { m_iLife = m_iStartLife; }

    /**
     * Rotation getter/setter
     */
    void setRot( const Quati &qRot ) {
        m_qRot = qRot;
    }
    const Quati& getRot() const { return m_qRot; }

    void bounce( const Vector3f &vColDir, float speed );

};