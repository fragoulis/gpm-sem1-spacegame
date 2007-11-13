
#pragma once
#include "movement.h"

namespace tlib 
{

    class OCLinearMovement : public IOCMovement
    {
    protected:
        // The velocity of the object
        float m_fVelocity;

    public:
        /**
         * Constructors
         */
        OCLinearMovement();
        OCLinearMovement( float fVel, const Vector3f& vDir );

        /**
         * Destructor
         */
        virtual ~OCLinearMovement(){}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("linearmovement"); 
        }

        /**
         * Returns the velocity of the object associated with this component
         */
        float getVelocity() const { return m_fVelocity; }

        /**
         * Sets the velocity of the object
         */
        void setVelocity( float vel ) { m_fVelocity = vel; }

        /**
         * Alters the velocity of the object
         */
        void addVelocity( float scalar ) { m_fVelocity += scalar; }

        /**
         * Updates the object's position
         */
        virtual void update();

    }; // end of OCLinearMovement class

} // end of namespace tlib