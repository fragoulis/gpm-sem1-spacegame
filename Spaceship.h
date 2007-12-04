#pragma once
#include "Object.h"
#include "SpotLight.h"

using namespace tlib;

// Forward declarations
class Tile3d;
class PSLaser;

/**
 * This is our Spaceship object
 */
class Spaceship : public Object
{
private:
    PSLaser *m_Laser;

    float
        // The spaceship's maximum velocity [Config]
        m_fMaxVelocity, 
        // This is the accelaration we are giving the spaceship 
        m_fVelFactor, 
        // 
        m_fRoll, m_fMaxRoll, m_fRollFactor,
        m_fRotFactor, m_fRotBias;

    bool m_bResetSpeed, m_bResetRoll;

    // The ship's spot light
    SpotLight m_Light;

public:
    /**
     * Constructor
     */
    Spaceship();

    /**
     * Initializes the spaceship by reading the configuration file
     * and setting components
     * This can also be called from within the constructor
     * NOTE: Error/Action logger doesn't not log contructor messages
     */
    void init();

    /**
     * Control the spaceship's speed
     */
    void speedUp();
    void speedDown();
    void resetSpeed();

    /**
     * Control the spaceship's movement
     */
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void resetRoll();

    /**
     * Fire actions
     */
    void fire();
    void ceaseFire();
    //void record();

    /**
     * Returns the health of the ship with a number between 0 and 100
     */
    int getHealth();

    /**
     * Updates all aspects of the spaceship
     */
    void update();

    /**
     * Applies the spotlight
     */
    void applyLight();

    /**
     * Returns the laser system associated with the spaceship
     */
    PSLaser *getLaser() { return m_Laser; }

}; // end Spaceship class