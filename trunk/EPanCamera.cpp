#include "EPanCamera.h"
#include "CameraMgr.h"

EPanCamera::EPanCamera( double dToFire ):
Event(dToFire)
{}

void EPanCamera::fire() const
{
	// Create a big explosion of particles
    CameraMgr::Instance().activate("pan-cam");
}