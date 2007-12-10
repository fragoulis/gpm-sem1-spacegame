#pragma once
#include "Camera.h"
using tlib::Vector3f;

class TPCamera : public Camera
{
protected:
    // The target object
    Object *m_oTarget;

    // The offset from the target object's position
    Vector3f m_vPosOffset;

    // This factor animates the camera following the spaceship
    // while it is speeding up
    float m_fPositionBias;

    // This factor animates the camera following the spaceship
    // after it has rotated
    float m_fRotationBias;

public:
    /**
     * Constructor
     */
    TPCamera();

    /**
     * 
     */
    void init( Object *oTarget );

    /**
     * Updates the position of the camera
     */
    void update();

    /**
     * Sets the target object
     */
    void setTarget( Object *oTarget ) {
        m_oTarget = oTarget;
    }

	/**
	 * Sets the position offset from the target
	 */
	void setPosOffset( const Vector3f& vOff ) {
		m_vPosOffset = vOff;
	}

    void onActive();

}; // end of TPCamera class
