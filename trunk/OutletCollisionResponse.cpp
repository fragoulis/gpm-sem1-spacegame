#include "OutletCollisionResponse.h"
#include "Outlet.h"
#include "Barrier.h"
#include "Animation.h"
using tlib::IOCAnimation;

void OutletCollisionResponse::respond( const Vector3f &vCollVector ) const
{
    Outlet *oOwner = (Outlet*)m_oOwner;

    // Stop outlet from blinking
    ((IOCAnimation*)oOwner->getComponent("animation"))->stop();
    // Start barrier animation
    ((IOCAnimation*)oOwner->getBarrier()->getComponent("animation"))->start();
}