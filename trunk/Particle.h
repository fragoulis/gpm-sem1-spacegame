#pragma once
#include "Vector3.h"
using tlib::Vector3f;

/**
 * Abstract class for the particles
 */
class Particle
{
private:
    // The particle's position
    Vector3f m_vPos;

public:
    /**
     * Getter/Setter for the position
     */
    const Vector3f getPos() const { 
        return m_vPos; 
    }

    void setPos( const Vector3f& vPos ) { 
        m_vPos = vPos; 
    }
};