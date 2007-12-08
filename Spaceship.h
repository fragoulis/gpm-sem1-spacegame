#pragma once
#include "Object.h"
#include "SpotLight.h"

using namespace tlib;

// Forward declarations
class Tile3d;
class PSLaser;
class SpaceshipShield;

/**
 * This is our Spaceship object
 */
class Spaceship : public Object
{
private:
    float
        // The spaceship's maximum velocity [Config]
        m_fMaxVelocity, 
        // This is the accelaration we are giving the spaceship 
        m_fVelFactor, 
        // 
        m_fRoll, m_fMaxRoll, m_fRollFactor,
        m_fRotFactor, m_fRotBias;

    bool m_bResetSpeed, m_bResetRoll;

    // Pointer to the ship's shield
    SpaceshipShield *m_oShield;

    // The ship's spot light
    SpotLight m_Light;

    // Ship's laser system
    PSLaser *m_Laser;

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
	void speed( float fAxis );
    void speedUp();
    void speedDown();
    void resetSpeed();

    /**
     * Control the spaceship's movement
     */
	void move( float fVert, float fHoriz );
	void resetMove();

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

    /**
     * Returns the ship's health and lives remaining
     */
    void getVitals( int &iHeatlhRatio, int &iLives, int &iMaxLives );

    /**
     * Updates all aspects of the spaceship
     */
    void update();

    /**
     * Applies the spotlight
     */
    void applyLight();

    /**
     * Accessords
     */
    PSLaser *getLaser() { return m_Laser; }
    const Object* getShield() const { return (Object*)m_oShield; }
    Object* getShield() { return (Object*)m_oShield; }
    void setShield( SpaceshipShield *oShield ) { m_oShield = oShield; }

}; // end Spaceship class