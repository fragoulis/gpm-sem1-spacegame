#pragma once

class PSCommon;
class PSTemplate
{
private:
    // The particle system's attributes
    float 
        m_fSize[2], 
        m_fVelocity[2],
        m_vfColor[4], 
        m_vfColorFactor[4];
    double 
        m_dReleaseTime,
        m_dSysDuration;
    int 
        m_iReleaseCount,
        m_iLifeSpan[2],
        m_iNumOfParticles;
    unsigned int m_uiTexId;

public:
    /**
     * Constructor
     */
    PSTemplate( const char *block );

    /**
     * Copies all values to the destination particle system
     */
    void copyTo( PSCommon &ps ) const;

};