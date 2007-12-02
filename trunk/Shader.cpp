#include <GL/GLee.h>
#include "Shader.h"
#include "Logger.h"

namespace tlib
{

    OCShader::OCShader():
    m_uiProg(0),
    m_bIsActive(0)
    {} 

    OCShader::OCShader( const char *vertex, const char *pixel ):
    m_bIsActive(0)
    {
        init( vertex, pixel );
    }

    OCShader::~OCShader()
    {}

    // ------------------------------------------------------------------------
    void OCShader::init( const char *vertex, const char *pixel )
    {
        m_uiProg = ShaderMgr::Instance().addProgram( vertex, pixel );
        if( !m_uiProg ) {
            _LOG("Shader program not loaded!");
        }
    }

    // ------------------------------------------------------------------------
    void OCShader::begin()
    {
        if( !m_bIsActive && m_uiProg ) 
        {
            glUseProgram( m_uiProg );
            m_bIsActive = true;
        }
    }

    // ------------------------------------------------------------------------
    void OCShader::end()
    {
        glUseProgram(0);
        m_bIsActive = false;
    }

} // end of namespace tlib