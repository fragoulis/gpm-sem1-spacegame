#include <windows.h>
#include <gl/gl.h>
#include "SimpleMaterial.h"

namespace tlib
{

    OCSimpleMaterial::OCSimpleMaterial(): 
        m_Ambient(Color(0.2f,0.2f,0.2f,1.0f)),
	    m_Diffuse(Color(0.8f,0.8f,0.8f,1.0f)),
        m_Specular(Color::black()),
        m_Emission(Color::black()),
        m_fShininess(0.0f)
    {}

    // ------------------------------------------------------------------------
    void OCSimpleMaterial::apply() const 
    {
        glMaterialfv( GL_FRONT,   GL_AMBIENT,   m_Ambient.rgba() );
        glMaterialfv( GL_FRONT,   GL_DIFFUSE,   m_Diffuse.rgba() );
        glMaterialfv( GL_FRONT,  GL_EMISSION,  m_Emission.rgba() );
        glMaterialfv( GL_FRONT,  GL_SPECULAR,  m_Specular.rgba() );
        glMaterialf(  GL_FRONT, GL_SHININESS,       m_fShininess );
    }

} // end of namespace tlib