
#pragma once

#include "orientation2d.h"

namespace tlib 
{

    class OCOrientation3D : public OCOrientation2D
    {
    protected:
        // The right vector
        Vector3f m_vRight;

    public:
        /**
         * Destructor
         */
        virtual ~OCOrientation3D(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("orientation3d"); 
        }

        /**
         * Returns the right vector
         */
        const Vector3f& getRight() const { return m_vRight; }
        Vector3f& getRight() { return m_vRight; }

        /**
         * Sets the right vector
         */
        void setRight( const Vector3f& vec ) { m_vRight = vec; }

        /**
         * Update right vector
         */
        void updateRight()
        {
            m_vRight = m_vView.cross( m_vUp );
            m_vRight.normalize();
        }

        virtual void pitch( const float fAngle )
        {
            rotate( fAngle, m_vRight );
        }

    }; // end of OCOrientation3D class

} // end of namespace tlib