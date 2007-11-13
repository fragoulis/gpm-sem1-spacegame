#pragma once
#include "VisualDisplayList.h"
#include "Vector3.h"

namespace tlib
{

    class OCVisualBox : public OCVisualDisplayList
    {
    private:
        // The boxes half dimensions
        Vector3f m_vHalfDim;

    public:
        /**
         * Constructor
         */
        OCVisualBox( const Vector3f& );

        /**
         * Destructor
         */
        virtual ~OCVisualBox() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("visualbox"); 
        }

        /**
         * Builds the cube object with textures using immediate mode
         */
        void buildObject() const;

        /**
         * Half dimensions getter/setter
         */
        const Vector3f& getHalfDim() const { return m_vHalfDim; }
        void setHalfDim( const Vector3f &vHalfDim ) {
            m_vHalfDim = vHalfDim;
        }

        /**
         * Whole dimensions getter/setter
         * Note: in the getter we return by value because the multiplication
         * that takes place returns a new vector. In addition we return const
         * for cohesion with the half dimension getter.
         */
        const Vector3f getDim() const { return m_vHalfDim * 2.0f; }
        void setDim( const Vector3f &vDim ) {
            m_vHalfDim = vDim * 0.5f;
        }

    }; // end of OCVisualBox class

} // end of namespace tlib