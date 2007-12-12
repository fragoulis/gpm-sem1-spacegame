#pragma once
#include "Singleton.h"
#include "Object.h"
using tlib::Object;

// ____________________________________________________________________________
// Waypoint structure
class Waypoint : public Object
{};

// ____________________________________________________________________________
// The autopilot class holds a number of waypoints to guide the spaceship
// through
class Autopilot : public Singleton<Autopilot>
{
    friend Singleton<Autopilot>;

private:
    bool m_bIsActive;
    Waypoint m_WPoints[3];
    Waypoint *m_CurrentWPoint;

public:
    void start() { 
        create();
        m_bIsActive = true; 
    }
    bool isActive() const { return m_bIsActive; }

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