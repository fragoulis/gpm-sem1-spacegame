#include "SingleTexture.h"
#include "gx/Image.h"
#include "Logger.h"
using gxbase::Image;

namespace tlib
{
    OCSingleTexture::OCSingleTexture() {}
    OCSingleTexture::OCSingleTexture( const char *filename ) {
        load( filename );
    }

    bool OCSingleTexture::load( const char *filename )
    {
        _LOG("Loading texture " + string(filename));
        Image image;

        // Load image file
        if( !image.Load( filename ) )
        {
            _LOG("Failed to load texture");
            image.Free();
            return false;
        }

        // Generate texture
        glGenTextures( 1, &m_TexId );

        // Generate mipmaps and free image from memory
        glBindTexture( GL_TEXTURE_2D, m_TexId );
        image.gluBuild2DMipmaps();
        image.Free();

        return true;
    }

} // end of namespace tlib