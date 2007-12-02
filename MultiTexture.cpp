#include <gl/glee.h>
#include "MultiTexture.h"
#include "Object.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "Logger.h"

namespace tlib
{
    OCMultiTexture::OCMultiTexture( unsigned int uiSize ) {
        m_vList.resize( uiSize, 0 );
        m_vNames.resize( uiSize );
    }

    OCMultiTexture::~OCMultiTexture()
    {}

    // ------------------------------------------------------------------------
    bool OCMultiTexture::set( unsigned int index, const char *filename )
    {
        _ASSERT(index>=0);
        if( index > m_vList.capacity() - 1 ) return false;

        // Get a handle for the texture
        GLuint texId = TextureMgr::Instance().getTexture( filename );
        if( texId ) {
            // Pass the id to the list
            m_vList[ index ] = texId;
            return true;
        }
        
        return false;
    }

    // ------------------------------------------------------------------------
    void OCMultiTexture::apply() const 
    {
        // Apply all textures one by one
        unsigned int i;
        for( i=0; i<m_vList.size(); ++i )
        {
            // Activate texture
            glActiveTexture( GL_TEXTURE0 + i );
            // Bind texture
            glBindTexture( GL_TEXTURE_2D, m_vList[i] );

            // See if the object has a shader component and if it has
            // check if it in use now
            OCShader *cShader = (OCShader*)getOwner()->getComponent("shader");
            if( cShader && cShader->isActive() )
            {
                _ASSERT(m_vNames[i].c_str());
                // Get the location of the texture in the shader an update it
                glUniform1i( cShader->getUniform( m_vNames[i].c_str() ), i );
            }
            else {
                // Just enable texturing for pipeline texturing
                glEnable( GL_TEXTURE_2D );
            }
        }
    }

    // ------------------------------------------------------------------------
    void OCMultiTexture::reset() const 
    {
        // Resets the active texture to zero and disables texturing
        unsigned int i;
        for( i=0; i<m_vList.size(); ++i )
        {
            glActiveTexture( GL_TEXTURE0 + (GLenum)m_vList.size() - 1 - (GLenum)i );
            glDisable( GL_TEXTURE_2D );
        }
    }

} // end of namespace tlib