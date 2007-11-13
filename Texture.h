
#pragma once
#include <windows.h>
#include <gl/gl.h>
#include "presentation.h"

namespace tlib
{

    class OCTexture : public IOCPresentation
    {
    private:
        unsigned int m_TexId;

    public:
        /**
         * Constructors
         */
        OCTexture();
        OCTexture( const char *filename );

        /**
         * Destructor
         */
        virtual ~OCTexture() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("texture"); 
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

    }; // end of OCTexture class

} // end of namespace tlib