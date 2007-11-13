#pragma once
#include "camera.h"

using tlib::Vector3f;

class TPCamera : public Camera
{
protected:
    // The target object
    Object *m_oTarget;

    // The offset from the target object's position
    Vector3f m_vPosOffset;

    //
    float m_fFollowBias;

    //
    bool m_bActive;

public:
    /**
     * Constructor
     */
    TPCamera();

    /**
     * Destructor
     */
    virtual ~TPCamera() {}

    /**
     * 
     */
    void setup( Object *oTarget );

    /**
     * Updates the position of the camera
     */
    virtual void update();

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

    /**
     *
     */
    void updateBias( float fDelta )
    {
        m_fFollowBias += 2.0f * fDelta;
        if( m_fFollowBias > 1.0f ) {
            m_bActive = false;
            m_fFollowBias = 1.0f;
        }
    }

    /**
     *
     */
    void activate() { 
        m_bActive = true;
        m_fFollowBias = 0.0f; 
    }

    /**
     *
     */
    void setActive( bool bActive ) { 
        m_bActive = bActive;
    }

}; // end of TPCamera class
