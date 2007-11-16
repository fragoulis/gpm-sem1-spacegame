#include "BladeAnimation.h"
#include "RotatingBlade.h"
#include "LinearMovement.h"
#include "Collision.h"
using namespace tlib;

// ----------------------------------------------------------------------------
void BladeAnimation::onUpdate()
{
    RotatingBlade *oRotBlade = (RotatingBlade*)getOwner();
    if( !oRotBlade->getBlade().isRotFactor() ) {
        oRotBlade->getBlade().slowDown();
        oRotBlade->getBlade().update();
    } else {
        IOCMovement *cMov = (IOCMovement*)oRotBlade->getComponent("movement");
        cMov->update();
    }

} // end onUpdate()

// ----------------------------------------------------------------------------
bool BladeAnimation::condition()
{
    // The animation must stop when the whole blade system has moved its
    // whole height
    if( m_fDistMoved > 2.0f * RotatingBlade::DistToDie ) {
        return true;
    }

    // Figure out along which axis we are moving
    OCLinearMovement *cMov = (OCLinearMovement*)m_oOwner->getComponent("movement");
    // The next line multiplies the position with the direction and then 
    // requests the length to get the current position of the object only 
    // for the axis along it is moving
    float fSystemPos = (getOwner()->getPos() * cMov->getDir()).length();

    m_fDistMoved += fabs(fSystemPos - m_fPrevPos);
    m_fPrevPos = fSystemPos;

    return false;
}

// ----------------------------------------------------------------------------
void BladeAnimation::onStart()
{
    // Deactivate collision with this barrier
    IOCCollision *cCol = (IOCCollision*)getOwner()->getComponent("collision");
    cCol->deactivate();

    // Initialize the helper variables
    // Set distance blade has moved to zero
    m_fDistMoved = 0.0f;

    // Set the current position
    OCLinearMovement *cMov = (OCLinearMovement*)m_oOwner->getComponent("movement");
    // The next line multiplies the position with the direction and then 
    // requests the length to get the current position of the object only 
    // for the axis along it is moving
    m_fPrevPos = (getOwner()->getPos() * cMov->getDir()).length();
}

// ----------------------------------------------------------------------------
void BladeAnimation::onStop()
{
}