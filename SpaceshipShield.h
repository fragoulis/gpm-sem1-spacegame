#pragma once
#include "TileObject.h"
using tlib::Vector3f;

class Timer;
class Spaceship;

class SpaceshipShield : public TileObject
{
private:
    // Pointer to the spaceship
    Spaceship *m_oShip;

    // A vector to hold the collision point between the shield a laser
    Vector3f m_vCollPoint;

    // Animation timers
    Timer *m_NoiseTimer, *m_GlowTimer;

public:
    /**
     * Initializes the shield object
     */
    void init( Spaceship *oShip );

    /**
     * Collision's point setter
     */
    void setCollPoint( const Vector3f& vCollPoint );

    /**
     * Updates the shield's position
     */
    void update();

    /**
     * Renders the shield.
     */
    void render() const;

    /**
     * Spaceship accessor
     */
    const Object* getShip() const { return (Object*)m_oShip; }
    Object* getShip() { return (Object*)m_oShip; }
};