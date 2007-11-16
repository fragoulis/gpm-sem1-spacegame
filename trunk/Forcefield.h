#pragma once
#include "Barrier.h"

class Forcefield : public Barrier
{
private:
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
    float getTTFactor() const { return m_fToTransparentFactor; }

};