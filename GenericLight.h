#pragma once
#include "TileObject.h"
#include "Color.h"
using tlib::Vector3f;

/** 
 * This class is an abstract base class that can describe a light.
 * It is used my the Light Manager to easily identify used/unused 
 * light sources and allocate them
 */
class GenericLight : public TileObject
{
public:
    // The types of lights
    enum LightType {
        POINT = 0,
        DIRECTIONAL,
        SPOT
    };

private:
    // The light's Id.
    // This is a number between 0 and 7
    short int m_iLightId;

    // The light's type
    LightType m_iType;

    // Flag that shows whether a light is used or not
    bool m_bIsOn;
    
    // The w-component
    // Here we save the w-component of the light's position.
    // If the light is a directional light, we set this 0, 
    // otherwise is 1.
    float m_fW;

    // A rendering offset, that allows us to have 
    // the visual part in a different position than
    // the actual lightsource
    Vector3f m_vRenderOffset;

protected:
    // Holds the constant attenuation for this light source
    float m_fConstantAttenuation;

    // Holds the linear attenuation for this light source
    float m_fLinearAttenuation;

    // Holds the quadratic attenuation for this light source
    float m_fQuadraticAttenuation;

    // The light's colors
    Color m_Ambient;
    Color m_Diffuse;
    Color m_Specular;

public:
    /**
     * Constructor
     */
    GenericLight();

    /**
     * Getter/Setter for the light id
     */
    short int getId() const { return m_iLightId; }
    void setId( int iLightId ) { m_iLightId = iLightId; }

    /**
     * Getter/Setter for the light type
     */
    LightType getLightType() const { return m_iType; }
    void setLightType( LightType iType ) {
        m_iType = iType;
        if( DIRECTIONAL == iType )
            m_fW = 0.0f;
    }

    /**
     * Offset getter/setter
     */
    const Vector3f& getRenderOffset() const {
        return m_vRenderOffset;
    }
    void setRenderOffset( float *offset ) {
        m_vRenderOffset.xyz( offset );
    }

    /**
     * Getters for the attenuations
     */
    float getConstantAttenuation() const { return m_fConstantAttenuation; }
    float getLinearAttenuation() const { return m_fLinearAttenuation; }
    float getQuadraticAttenuation() const { return m_fQuadraticAttenuation; }

    /**
     * Colors' accessors
     */
    const Color& getAmbient() const { return m_Ambient; }
    Color& getAmbient() { return m_Ambient; }
    const Color& getDiffuse() const { return m_Diffuse; }
    Color& getDiffuse() { return m_Diffuse; }
    const Color& getSpecular() const { return m_Specular; }
    Color& getSpecular() { return m_Specular; }

    /**
     * On/Off controller
     */
    void turnOn();
    void turnOff();
    bool isOn() const { return m_bIsOn; }

    /**
     * Positions the light source
     * Alternatively, if the light is directional, this will set
     * the light's direction
     */
    void setPos( const Vector3f &vPos );

    /**
     * Setter for the light's attributes.
     * These functions save the values to the member variables
     * and also apply them to the light source.
     */
    void setAmbient( const Color &ambient );
    void setDiffuse( const Color &diffuse );
    void setSpecular( const Color &specular );
    void setConstantAttenuation( float fConstantAttenuation );
    void setLinearAttenuation( float fLinearAttenuation );
    void setQuadraticAttenuation( float fQuadraticAttenuation );

    /**
     * Resets the light's attributes
     */
    virtual void reset();

    /**
     * Applies the position of the light
     */
    void apply() const;

}; // end of GenericLight class