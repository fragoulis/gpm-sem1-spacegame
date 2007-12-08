#include "gl/glee.h"
#include "SingleTexture.h"
#include "Object.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "Logger.h"

namespace tlib
{
    OCSingleTexture::OCSingleTexture() {}
    OCSingleTexture::OCSingleTexture( const char *filename ) {
        load( filename );
    }

    // ------------------------------------------------------------------------
    bool OCSingleTexture::load( const char *filename )
    {
        // Get a handle for the texture
        m_uiTexId = TextureMgr::Instance().getTexture( filename );

        return (0==m_uiTexId)?false:true;
    }

    // ------------------------------------------------------------------------
    void OCSingleTexture::apply() const 
    {
        // Enable pipeline texture rendering
        glEnable( GL_TEXTURE_2D );
        // Bind texture
        glBindTexture( GL_TEXTURE_2D, m_uiTexId );
        
        // See if the object has a shader component and if it has
        // check if it in use now
        OCShader *cShader = (OCShader*)getOwner()->getComponent("shader");
        if( cShader && cShader->isActive() )
        {
            _ASSERT(m_sName.c_str());
            // Get the location of the texture in the shader an update it
            glUniform1i( cShader->getUniform( m_sName.c_str() ), 0 );
        }
    }

    // ------------------------------------------------------------------------
    void OCSingleTexture::reset() const {
        glDisable( GL_TEXTURE_2D );
    }

} // end of namespace tlib