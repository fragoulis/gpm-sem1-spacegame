#pragma once
#include "texture.h"

namespace tlib
{

    class OCSingleTexture : public IOCTexture
    {
    private:
        // The texture id
        unsigned int m_uiTexId;

        // The texture name
        string m_sName;

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
         * Sets the texture's name for use in shaders
         */
        void setName( const char *sName ) {
            m_sName = sName;
        }

        /**
         * Loads the texture into memory
         */
        bool load( const char *filename );

        /**
         * Applies the texture
         */
        void apply() const;

        /**
         * Disables the texture
         */
        void reset() const;

    }; // end of OCSingleTexture class

} // end of namespace tlib