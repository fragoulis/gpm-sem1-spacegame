#include "OutletCollisionResponse.h"
#include "Outlet.h"
#include "Barrier.h"
#include "Animation.h"
using tlib::IOCAnimation;

void OutletCollisionResponse::respond( const Vector3f &vCollVector ) const
{
    Outlet *oOwner = (Outlet*)m_oOwner;

    // We only want to do this reaction once, so we limit
    // this by setting the barrier's pointer to null after the 
    // first time
    if( oOwner->getBarrier() )
    {
        // Stop outlet from blinking
        ((IOCAnimation*)oOwner->getComponent("animation"))->stop();
        // Start barrier animation
        ((IOCAnimation*)oOwner->getBarrier()->getComponent("animation"))->start();
        // Set outlet's target pointer to null
        oOwner->setBarrier(0);
    }
}