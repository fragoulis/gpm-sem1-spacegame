#pragma once
#include "TileObject.h"
using namespace tlib;

class PSLaser;

/**
 * This is our turret object
 */
class Turret : public TileObject
{
private:
    PSLaser *m_Laser;

public:
    /**
     * Initializes a turret object
     */
    void init();

    /**
     * Getter for the laser system
     */
    const PSLaser* getLaser() const { 
        return m_Laser; 
    }
    PSLaser* getLaser() { return m_Laser; }

}; // end Spaceship class