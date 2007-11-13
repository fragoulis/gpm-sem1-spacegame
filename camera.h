
#pragma once

#include "object.h"
using tlib::Object;

class Camera : public Object
{
public:
    /**
     * Constructor
     */
    Camera();

    /**
     * Destructor
     */
    virtual ~Camera() {}

    /**
     * Applies the view changes to the modelview matrix
     */
    void apply();

    /**
     * Writes the camera's significant points and vectors to a stream
     */
    void write( std::ostream &cout );

}; // end of Camera class