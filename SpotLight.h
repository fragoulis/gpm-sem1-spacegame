#include "GenericLight.h"

class SpotLight : public GenericLight
{
private:
    // The light's direction
    Vector3f m_vDir;

    // The light's cutoff 
    float m_fCutOff;

    // The light's exponent
    float m_fExponent;

public:
    /**
     * Constructor
     */
    SpotLight();

    /**
     * Setter for the light's attributes.
     * These functions save the values to the member variables
     * and also apply them to the light source.
     */
    void setDir( const Vector3f &vDir );
    void setCutOff( float fCutOff );
    void setExponent( float fExponent );

    /**
     * Resets the light's attributes
     */
    void reset();
};