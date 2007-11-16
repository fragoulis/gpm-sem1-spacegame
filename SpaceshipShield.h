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
};