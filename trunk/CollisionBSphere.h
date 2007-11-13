#pragma once
#include "Collision.h"
#include "Object.h"

namespace tlib 
{
    class OCCollisionBSphere : public IOCCollision
    {
    protected:
        int m_iRadius;

    public:
        /**
         * Constructor
         */
        OCCollisionBSphere();
        OCCollisionBSphere( int );

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
        int getRadius() const {
            return m_iRadius;
        }

        /**
         * Sets the radius
         */
        void setRadius( int iRadius ) {
            m_iRadius = iRadius;
        }

    }; // end of OCCollisionGXModel class

} // end of namespace tlib