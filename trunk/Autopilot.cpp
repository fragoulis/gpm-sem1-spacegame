#include "Autopilot.h"
#include "ObjectMgr.h"
#include "Tilemap.h"

Autopilot::Autopilot():
m_CurrentWPoint(0),
m_bIsActive(false)
{
    m_WPoints[0].deactivate();
    m_WPoints[1].deactivate();
    m_WPoints[2].deactivate();
}

    // if we are above the height of the reactof
    //   return nearest way point
    //   push nearest waypoint
    // push exitdoor waypoint
    // push far space waypoint
    // activate camera to show spacestation
// ----------------------------------------------------------------------------
void Autopilot::create()
{
    m_CurrentWPoint = &m_WPoints[0];
}