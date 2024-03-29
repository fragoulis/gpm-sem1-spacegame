#include "TextureMgr.h"
#include "Logger.h"
using tlib::Logger;

TextureMgr::TextureMgr():
m_iFormat(Image::RGB)
{}

TextureMgr::~TextureMgr()
{}

// ----------------------------------------------------------------------------
GLuint TextureMgr::getTexture( const char *filename )
{
    GLuint uiTexId;
    string sTexture( filename );

    // Check is the requested texture is already loaded
    // and if it is return its handle.
    uiTexId = m_vTextures[ sTexture ];
    if( uiTexId ) {
        _LOG("Texture already loaded "+ string(filename) +"["+ toStr<GLuint>(uiTexId) +"]");
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
        return uiTexId;
    }

    // Now try, to load the texture using the gxbase image class
    Image image;

    // Load image file
    if( !image.Load( filename ) )
    {
        // Return nothing if this fails
        _LOG("Failed to load texture "+ sTexture);
        image.Free();
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
        return 0;
    }

    // Change the format if necessary
    if( m_iFormat != Image::RGB ) {
        image.SetFormat( m_iFormat );
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
    }

    // Generate texture
    glGenTextures( 1, &uiTexId );

    _LOG("Loaded texture "+ string(filename) +"["+ toStr<GLuint>(uiTexId) +"]");

    // Generate mipmaps and free image from memory
    glBindTexture( GL_TEXTURE_2D, uiTexId );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    image.gluBuild2DMipmaps();
    image.Free();

    // Push it to the texture list
    m_vTextures[ sTexture ] = uiTexId;

    return uiTexId;
}