#pragma once
#include "Singleton.h"
#include "Object.h"
using tlib::Object;

// ____________________________________________________________________________
// Waypoint structure
class Waypoint : public Object
{};

static const int MAX_WPOINTS = 4;
// ____________________________________________________________________________
// The autopilot class holds a number of waypoints to guide the spaceship
// through
class Autopilot : public Singleton<Autopilot>
{
    friend Singleton<Autopilot>;

private:
    bool m_bIsActive;
    Waypoint m_WPoints[MAX_WPOINTS];
    unsigned m_uiCurWPoint;
    float m_vfShipEndPos[3];

public:
    void start() { 
        create();
        m_bIsActive = true; 
    }
    bool isActive() const { return m_bIsActive; }

    /**
     * Updates the spaceship's orientation in order to reach the 
     * next waypoint
     */
    void update();

private:
    /**
     * Constructor
     */
    Autopilot();

    /**
     * Creates the waypoints based on the spaceship's and the reactor's position
     */
    void create();
};