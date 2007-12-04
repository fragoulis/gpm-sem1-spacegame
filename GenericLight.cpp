#include <windows.h>
#include <gl/gl.h>
#include "LightMgr.h"
#include "GenericLight.h"

GenericLight::GenericLight():
m_fW(1.0f),
m_iLightId(-1),
m_bIsOn(false),
m_Ambient(Color( 0.0f, 0.0f, 0.0f, 1.0f )),
m_Diffuse(Color( 1.0f, 1.0f, 1.0f, 1.0f )),
m_Specular(Color( 1.0f, 1.0f, 1.0f, 1.0f )),
m_fConstantAttenuation(1.0f),
m_fLinearAttenuation(0.0f),
m_fQuadraticAttenuation(0.0f)
{
    // Set object type
    setType( LIGHT );
}

// ----------------------------------------------------------------------------
void GenericLight::reset() 
{
    glLightfv( GL_LIGHT0+m_iLightId,  GL_AMBIENT, m_Ambient.rgba() );
    glLightfv( GL_LIGHT0+m_iLightId,  GL_DIFFUSE, m_Diffuse.rgba() );
    glLightfv( GL_LIGHT0+m_iLightId, GL_SPECULAR, m_Diffuse.rgba() );
    glLightf(GL_LIGHT0+m_iLightId,  GL_CONSTANT_ATTENUATION, m_fConstantAttenuation );
    glLightf(GL_LIGHT0+m_iLightId,    GL_LINEAR_ATTENUATION, m_fLinearAttenuation   );
    glLightf(GL_LIGHT0+m_iLightId, GL_QUADRATIC_ATTENUATION, m_fQuadraticAttenuation);
}

// ----------------------------------------------------------------------------
bool GenericLight::findId()
{
    return LightMgr::Instance().initLight( this );
}

// ----------------------------------------------------------------------------
void GenericLight::turnOff()
{
    // Disable the light source
    glDisable( GL_LIGHT0 + m_iLightId );
    // Set flag to false
    m_bIsOn = false;
}

// ----------------------------------------------------------------------------
void GenericLight::turnOn()
{
    // Set flag to true
    m_bIsOn = true;
    // Enable lighting
    glEnable( GL_LIGHTING );
    // Enable the light source
    glEnable( GL_LIGHT0 + m_iLightId );
}

// ----------------------------------------------------------------------------
void GenericLight::setPos( const Vector3f &vPos )
{
    // Update the object's position
    Object::setPos( vPos );

    // Update the light's position
    float vfPos[] = { vPos.x(), vPos.y(), vPos.z(), m_fW };
    glLightfv( GL_LIGHT0+m_iLightId, GL_POSITION, vfPos );
}

// ----------------------------------------------------------------------------
void GenericLight::setAmbient( const Color &ambient ) {
	m_Ambient = ambient;
	glLightfv(GL_LIGHT0+m_iLightId, GL_AMBIENT, m_Ambient.rgba());
}		

// ----------------------------------------------------------------------------
void GenericLight::setDiffuse( const Color &diffuse ) {
	m_Diffuse = diffuse;
	glLightfv( GL_LIGHT0+m_iLightId, GL_DIFFUSE, m_Diffuse.rgba() );
}

// ----------------------------------------------------------------------------
void GenericLight::setSpecular( const Color &specular ) {
	m_Specular = specular;
	glLightfv( GL_LIGHT0+m_iLightId, GL_SPECULAR, m_Diffuse.rgba() );
}

// ----------------------------------------------------------------------------
void GenericLight::setConstantAttenuation( float fConstantAttenuation ) {
    m_fConstantAttenuation = fConstantAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_CONSTANT_ATTENUATION, m_fConstantAttenuation);
}

// ----------------------------------------------------------------------------
void GenericLight::setLinearAttenuation( float fLinearAttenuation ) {
    m_fLinearAttenuation = fLinearAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_LINEAR_ATTENUATION, m_fLinearAttenuation);
}

// ----------------------------------------------------------------------------
void GenericLight::setQuadraticAttenuation( float fQuadraticAttenuation ) {
    m_fQuadraticAttenuation = fQuadraticAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_QUADRATIC_ATTENUATION, m_fQuadraticAttenuation);
}