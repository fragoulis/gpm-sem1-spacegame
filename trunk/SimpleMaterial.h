#pragma once
#include "Material.h"
#include "Color.h"

namespace tlib
{

    class OCSimpleMaterial : public IOCMaterial
    {
    private:
        // Ambient color
        Color m_Ambient;

        // Diffuse color
        Color m_Diffuse;

        // Emission color
        Color m_Emission;

        // Specular color
        Color m_Specular;

        // Specular shininess
        float m_fShininess;

    public:
        /**
         * Constructors
         */
        OCSimpleMaterial();

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

        const Color& getSpecualar() const { return m_Specular; }
        Color& getSpecualar() { return m_Specular; }

        float getShininess() const { return m_fShininess; }

        /**
         * Assignment methods for diffuse, ambient and emission
         */
        void setAmbient( const Color &ambient ) { m_Ambient = ambient; }
        void setDiffuse( const Color &diffuse ) { m_Diffuse = diffuse; }
        void setEmission( const Color &emission ) { m_Emission = emission; }
        void setSpecular( const Color &specular ) { m_Specular = specular; }
        void setShininess( float fShininess ) { m_fShininess = fShininess; }

    }; // end of OCSimpleMaterial class

} // end of namespace tlib