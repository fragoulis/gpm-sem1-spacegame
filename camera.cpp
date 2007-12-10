#include <windows.h>
#include <gl/glu.h>
#include "Camera.h"
#include "QuatRotation.h"

using namespace tlib;

/**
 * Constructor
 */
Camera::Camera()
{
    // Position camera at (0,0,1)
    getPos().xyz( 0.0f, 0.0f, 1.0f );

    // Initialize orientation component
    OCQuatRotation *cOrientation = new OCQuatRotation;
    // look at (0,0,0)
    cOrientation->setView( Vector3f(0.0f,0.0f,-1.0f) );
    cOrientation->setUp( Vector3f(0.0f,1.0f,0.0f) );
    cOrientation->setRight( Vector3f(1.0f,0.0f,0.0f) );
    setComponent( cOrientation );
}

Camera::~Camera()
{}

// ----------------------------------------------------------------------------
void Camera::apply()
{
    // Get viewing and up vectors
    OCOrientation3D *cOri = (OCOrientation3D*)getComponent("orientation");
    const Vector3f& vCenter = getPos() + cOri->getView();
    const Vector3f& vUp = cOri->getUp();

    gluLookAt( getPos().x(), getPos().y(), getPos().z(), 
                vCenter.x(),  vCenter.y(),  vCenter.z(),
                    vUp.x(),      vUp.y(),      vUp.z() );
}

// ----------------------------------------------------------------------------
void Camera::write( std::ostream &cout )
{
    // Get viewing and up vectors
    OCOrientation3D *cOri = (OCOrientation3D*)getComponent("orientation");

    cout << "Eye"  << getPos()        << std::endl;
    cout << "View" << cOri->getView() << std::endl;
    cout << "Up"   << cOri->getUp()   << std::endl;
}

// ----------------------------------------------------------------------------
void Camera::update()
{}