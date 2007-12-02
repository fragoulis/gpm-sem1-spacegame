    #include "SingleTexture.h"
#include "TextureMgr.h"
#include "Logger.h"

namespace tlib
{
    OCSingleTexture::OCSingleTexture() {}
    OCSingleTexture::OCSingleTexture( const char *filename ) {
        load( filename );
    }

    bool OCSingleTexture::load( const char *filename )
    {
        //_LOG("Loading texture " + string(filename));
        //Image image;

        //// Load image file
        //if( !image.Load( filename ) )
        //{
        //    _LOG("Failed to load texture");
        //    image.Free();
        //    return false;
        //}
        //
        //// Generate texture
        //glGenTextures( 1, &m_uiTexId );

        //// Generate mipmaps and free image from memory
        //glBindTexture( GL_TEXTURE_2D, m_uiTexId );
        //image.gluBuild2DMipmaps();
        //image.Free();
        
        //return true;

        // Get a handle for the texture
        m_uiTexId = TextureMgr::Instance().getTexture( filename );

        return (0==m_uiTexId)?false:true;
    }

} // end of namespace tlib