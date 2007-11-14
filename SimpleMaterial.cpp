#include <windows.h>
#include <gl/gl.h>
#include "SimpleMaterial.h"

namespace tlib
{

    OCSimpleMaterial::OCSimpleMaterial(): 
        m_Ambient(Color::black()),
	    m_Diffuse(Color::white())
    {}
    OCSimpleMaterial::OCSimpleMaterial( const Color &ambient, 
                                        const Color &diffuse, 
                                        const Color &emission ):
        m_Ambient(ambient),
        m_Diffuse(diffuse),
        m_Emission(emission)
    {}

    // ------------------------------------------------------------------------
    void OCSimpleMaterial::create( const Color &ambient, 
                                   const Color &diffuse, 
                                   const Color &emission) 
    {
	    setAmbient(ambient);
	    setDiffuse(diffuse);
	    setEmission(emission);
    }

    // ------------------------------------------------------------------------
    void OCSimpleMaterial::apply() const 
    {
        glMaterialfv( GL_FRONT, GL_AMBIENT, m_Ambient.rgba() );
        glMaterialfv( GL_FRONT, GL_DIFFUSE, m_Diffuse.rgba() );
        glMaterialfv( GL_FRONT, GL_EMISSION, m_Emission.rgba() );
    }

} // end of namespace tlib