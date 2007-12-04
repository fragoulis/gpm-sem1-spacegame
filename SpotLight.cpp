#include <windows.h>
#include <gl/gl.h>
#include "SpotLight.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
using tlib::OCSimpleMaterial;
using tlib::OCVisualVertexArraySphere;

SpotLight::SpotLight():
m_vDir(Vector3f( 0.0f, 0.0f, 1.0f )),
m_fCutOff(180.0f),
m_fExponent(0.0f)
{
    // Set the light type
    setLightType( SPOT );

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color::black() );
    cMat->setDiffuse( Color::black() );
    cMat->setEmission( Color::red() );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( 1.5, 15, 15 ) );
}

// ----------------------------------------------------------------------------
void SpotLight::reset() 
{
    glLightfv( GL_LIGHT0+getId(), GL_SPOT_DIRECTION, m_vDir.xyz() );
    glLightf ( GL_LIGHT0+getId(),    GL_SPOT_CUTOFF,    m_fCutOff );
    glLightf ( GL_LIGHT0+getId(),  GL_SPOT_EXPONENT,  m_fExponent );
    GenericLight::reset();
}

// ----------------------------------------------------------------------------
void SpotLight::setDir( const Vector3f &vDir ) {
	m_vDir = vDir;
	glLightfv( GL_LIGHT0+getId(), GL_SPOT_DIRECTION, m_vDir.xyz() );
}

// ----------------------------------------------------------------------------
void SpotLight::setCutOff( float fCutOff ) {
    m_fCutOff = fCutOff;
    glLightf(GL_LIGHT0+getId(), GL_SPOT_CUTOFF, m_fCutOff);
}

// ----------------------------------------------------------------------------
void SpotLight::setExponent( float fExponent ) {
    m_fExponent = fExponent;
    glLightf(GL_LIGHT0+getId(), GL_SPOT_EXPONENT, m_fExponent);
}   