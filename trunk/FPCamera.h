#pragma once
#include "Camera.h"
using tlib::Vector3f;

class FPCamera : public Camera
{
protected:
    // The target object
    Object *m_oTarget;

    // The offset from the target object's position
    Vector3f m_vPosOffset;

public:
    /**
     * Constructor
     */
    FPCamera();

    /**
     * 
     */
    void init( Object *oTarget );

    /**
     * Updates the position of the camera
     */
    void update();

    void onActive();

}; // end of FPCamera class
