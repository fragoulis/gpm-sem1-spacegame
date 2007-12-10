#pragma once
#include "Object.h"
using tlib::Object;
using tlib::Vector3f;

class Camera : public Object
{
protected:
    // The camera's previous position
    Vector3f m_vPrevPos;

public:
    /**
     * Constructor
     */
    Camera();

    /**
     * Destructor
     */
    virtual ~Camera();

    /**
     * Previous position getter
     */
    const Vector3f& getPrevPos() const { return m_vPrevPos; }
    Vector3f& getPrevPos() { return m_vPrevPos; }

    /**
     * Applies the view changes to the modelview matrix
     */
    void apply();

    /**
     * Writes the camera's significant points and vectors to a stream
     */
    void write( std::ostream &cout );

    /**
     * A virtual update method for the derived instances that
     * require an constant update
     */
    virtual void update();

}; // end of Camera class