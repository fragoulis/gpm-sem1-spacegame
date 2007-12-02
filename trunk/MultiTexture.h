#pragma once
#include "texture.h"
#include <vector>
using std::vector;

namespace tlib
{
    typedef unsigned int    texid_t;
    typedef vector<texid_t> TextureList;
    typedef vector<string>  TextureNames;

    class OCMultiTexture : public IOCTexture
    {
    private:
        // The list of texture ids
        TextureList m_vList;

        // The list with names to associate every texture
        // with a sampler in a shader
        TextureNames m_vNames;

    public:
        /**
         * Constructors
         */
        OCMultiTexture( size_t uiSize );

        /**
         * Destructor
         */
        virtual ~OCMultiTexture();

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("multitexture"); 
        }

        /**
         * Returns the number of textures used by this component
         */
        size_t getNumOfTextures() const {
            return m_vList.size();
        }

        /**
         * Loads a texture into memory and pushes
         * its id to the texture id list
         */
        bool set( texid_t index, const char *filename );

        /**
         * Set a name for an already loaded texture
         */
        bool setName( texid_t index, const char *sName ) 
        {
            _ASSERT(index>=0);
            if( index > m_vNames.capacity() - 1 ) return false;

            m_vNames[ index ] = sName;
            return true;
        }

        /**
         * Applies the textures
         */
        void apply() const;

        /**
         * Disables the textures
         */
        void reset() const;

    }; // end of OCSingleTexture class

} // end of namespace tlib