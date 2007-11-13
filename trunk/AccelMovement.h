
#pragma once
#include "LinearMovement.h"

namespace tlib 
{

    class OCAccelMovement : public OCLinearMovement
    {
    protected:
        // The velocity of the object
        float m_fAccel;

    public:
        /**
         * Constructors
         */
        OCAccelMovement();
        OCAccelMovement( float fAcc, float fVel, const Vector3f& vDir );

        /**
         * Destructor
         */
        virtual ~OCAccelMovement(){}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("accelmovement"); 
        }

        /**
         * Returns the acceleration of the object associated with this component
         */
        float getAccel() const { return m_fAccel; }

        /**
         * Sets the acceleration of the object
         */
        void setAccel( float acc ) { m_fAccel = acc; }

        /**
         * Alters the acceleration of the object
         */
        void addAccel( float scalar ) { m_fAccel += scalar; }

        /**
         * Updates the object's velocity and position
         */
        virtual void update();

    }; // end of OCAccelMovement class

} // end of namespace tlib