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
    Particle *m_pNext;

    // The particle's position
    Vector3f m_vPos;
    Vector3f m_vPrevPos;

    // Particle's velocity
    float m_fScalarVelocity;
    Vector3f m_vVelocity;

    // The particle's rotation
    // [Can be replaced by a 4d vector]
    Quati m_qRot;

    // The size of the particle
    float m_fSize;

    // Particle's life
    int m_iLife;
    int m_iStartLife;

    // Particle's color
    float m_vfColor[4];

public:
    /**
     * Next particle pointer getter/setter
     */
    void setNext( Particle *next ) {
        m_pNext = next;
    }
    Particle* getNext() const {
        return m_pNext;
    }

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
        m_vVelocity = vDir * m_fScalarVelocity;
    }

    /**
     * Sets the scalar velocity of the particle
     */
    void setVelocity( float fVelocity ) {
        m_fScalarVelocity = fVelocity;
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

    /**
     * Color getter/setter
     */
    void setColor( float color[] ) { 
        m_vfColor[0] = color[0];
        m_vfColor[1] = color[1];
        m_vfColor[2] = color[2];
        m_vfColor[3] = color[3];
    }
    const float* getColor() const { return m_vfColor; }
    
    void updateColor( float vfStart[], float vfFactor[] ) 
    {
        const float rationInv = 1.0f - getLifeRatio();
        m_vfColor[0] = vfStart[0] + vfFactor[0] * rationInv;
        m_vfColor[1] = vfStart[1] + vfFactor[1] * rationInv;
        m_vfColor[2] = vfStart[2] + vfFactor[2] * rationInv;
        m_vfColor[3] = vfStart[3] + vfFactor[3] * rationInv;
    }

    /**
     * Gives a new direction for the particle based on the collision
     * vector
     */
    void bounce( const Vector3f &vColDir, float speed );

};