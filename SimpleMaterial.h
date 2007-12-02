#pragma once
#include "Material.h"
#include "Color.h"

namespace tlib
{

    class OCSimpleMaterial : public IOCMaterial
    {
    private:
        // ambient material
        Color m_Ambient;

        // diffuse material
        Color m_Diffuse;

        // emission material
        Color m_Emission;

    public:
        /**
         * Constructors
         */
        OCSimpleMaterial();
        OCSimpleMaterial( const Color &ambient, 
                          const Color &diffuse, 
                          const Color &emission );

        /**
         * Destructor
         */
        virtual ~OCSimpleMaterial() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("simplematerial"); 
        }

        /**
         * Creates the material
         */
        void create( const Color &ambient, 
                     const Color &diffuse, 
                     const Color &emission );
        /**
         * Applies the material
         */
        virtual void apply() const;

        /**
         * Accessor methods for diffuse, ambient and emission
         */
        const Color& getAmbient() const { return m_Ambient; }
        Color& getAmbient() { return m_Ambient; }

        const Color& getDiffuse() const { return m_Diffuse; }
        Color& getDiffuse() { return m_Diffuse; }

        const Color& getEmission() const { return m_Emission; }
        Color& getEmission() { return m_Emission; }

        /**
         * Assignment methods for diffuse, ambient and emission
         */
        void setAmbient( const Color &ambient ) { m_Ambient = ambient; }
        void setDiffuse( const Color &diffuse ) { m_Diffuse = diffuse; }
        void setEmission( const Color &emission ) { m_Emission = emission; }

    }; // end of OCSimpleMaterial class

} // end of namespace tlib