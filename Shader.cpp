#include "gl/glee.h"
#include "Shader.h"
#include "Logger.h"

namespace tlib
{

    OCShader::OCShader( ShaderMgr::ShaderProgram iType ):
    m_bIsActive(0)
    {
        m_uiProg = ShaderMgr::Instance().getProgram( iType );
    }

    OCShader::~OCShader()
    {}

    // ------------------------------------------------------------------------
    void OCShader::begin()
    {
        if( !m_bIsActive && m_uiProg ) 
        {
            glUseProgram( m_uiProg );
            m_bIsActive = true;

            //glUniform1f( getUniform("invRadius"), fInvRadius );
        }
    }

    // ------------------------------------------------------------------------
    void OCShader::end()
    {
        glUseProgram(0);
        m_bIsActive = false;
    }

} // end of namespace tlib