#pragma once
#include "Barrier.h"

class Forcefield : public Barrier
{
private:
    // The forcefield's alpha channer value
    float m_fAlpha;

    // How quickly will the forcefield become totaly transparent
    float m_fToTransparentFactor;

public:
    /**
     * Constructor
     */
    Forcefield();

    /** 
     * Returns the to transparent factor
     */
    //float getTTFactor() const { return m_fToTransparentFactor; }

    /**
     * Decreases the forcefield's alpha value
     */
    void fade( float delta ) {
        m_fAlpha -= m_fToTransparentFactor * delta;
    }

    /**
     * Returns the alpha value
     */
    float getAlpha() const { return m_fAlpha; }

};