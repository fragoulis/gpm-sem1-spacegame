#pragma once
#include "Collision.h"
#include "Object.h"

namespace tlib 
{
    class OCCollisionBSphere : public IOCCollision
    {
    protected:
        float m_fRadius;

    public:
        /**
         * Constructor
         */
        OCCollisionBSphere();
        OCCollisionBSphere( float fRadius );

        /**
         * Destructor
         */
        virtual ~OCCollisionBSphere(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("collisionbsphere"); 
        }

        /**
         * Returns the bounding sphere's radius
         */
        float getRadius() const {
            return m_fRadius;
        }

        /**
         * Sets the radius
         */
        void setRadius( float fRadius ) {
            m_fRadius = fRadius;
        }

    }; // end of OCCollisionGXModel class

} // end of namespace tlib