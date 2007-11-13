#pragma once
#include "Collision.h"
#include "Object.h"

namespace tlib 
{
    class OCCollisionBBox : public IOCCollision
    {
    protected:
        Vector3f m_BBox;

    public:
        /**
         * Constructor
         */
        OCCollisionBBox();
        OCCollisionBBox( const Vector3f& );

        /**
         * Destructor
         */
        virtual ~OCCollisionBBox(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("collisionbbox"); 
        }

        /**
         * Returns the bounding box vector
         */
        const Vector3f& getBBox() const {
            return m_BBox;
        }

        /**
         * Sets the bounding box vector
         */
        void setBBox( const Vector3f& vBBox ) {
            m_BBox = vBBox;
        }

    }; // end of OCCollisionGXModel class

} // end of namespace tlib