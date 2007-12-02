#pragma once
#include "TileObject.h"

class Spaceship;

class SpaceshipShield : public TileObject
{
private:
    // Pointer to the spaceship
    Spaceship *m_oShip;

public:
    /**
     * Initializes the shield object
     */
    void init( Spaceship *oShip );

    /**
     * Updates the shield's position
     */
    void update();

    /**
     * Returns a base pointer of the spaceship
     */
    const Spaceship* getShip() const { return (Spaceship*)m_oShip; }
    Spaceship* getShip() { return (Spaceship*)m_oShip; }
};