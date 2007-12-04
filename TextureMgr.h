#pragma once
#include <map>
#include <string>
#include "Singleton.h"
#include "GX/Image.h"

using std::map;
using std::string;
using gxbase::Image;

// Map a string to an opengl texture id
typedef map< string, unsigned int >  TextureList;

class TextureMgr : public Singleton<TextureMgr>
{
    // Declare singleton as a friend so that its destructor
    // call call the manager's destructor
    friend Singleton<TextureMgr>;

private:
    // The associative array of textures
    TextureList m_vTextures;

    // Always defaults to Image::RGB
    Image::Format m_iFormat;

private:
    /**
     * Constructor
     */
    TextureMgr();

    /**
     * Destructor
     */
    ~TextureMgr();

public:
    /**
     * This function is responsible of loading a texture by its
     * filename. If a same filename already exists, it does not
     * load anything. 
     * It returns the handle for the requested texture.
     */
    GLuint getTexture( const char *filename );

    /**
     * Format setter
     */
    void setFormat( Image::Format iFormat ) {
        m_iFormat = iFormat;
    }

}; // end of TextureMgr class