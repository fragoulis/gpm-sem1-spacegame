#pragma once
#include <windows.h>
#include <gl/gl.h>
#include "texture.h"

namespace tlib
{

    class OCSingleTexture : public IOCTexture
    {
    private:
        unsigned int m_TexId;

    public:
        /**
         * Constructors
         */
        OCSingleTexture();
        OCSingleTexture( const char *filename );

        /**
         * Destructor
         */
        virtual ~OCSingleTexture() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("singletexture"); 
        }

        /**
         * Loads the texture into memory
         */
        bool load( const char *filename );

        /**
         * Applies the material
         */
        void apply() const 
        {
            glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, m_TexId );
        }

    }; // end of OCSingleTexture class

} // end of namespace tlib