#pragma once
#include "Object.h"

using namespace tlib;

// Forward declarations
class TPCamera;
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

    bool 
        m_bResetSpeed, m_bResetRoll,
        // Collision detection flags
        // These flags will help speed up collision detection by cutting out
        // the objects we have to compare the spaceship with
        m_bInStation, m_bInReactor;


    TPCamera *m_TPCamera;
    Tile3d *m_CurrentTile;

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
    void setup();

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
     * Other actions
     */
    void fire();
    void ceaseFire();
    //void toggleCam();
    //void record();

    /**
     * Sets the third-person camera 
     */
    void setCamera( TPCamera *cam ) {
        m_TPCamera = cam;
    }

    /**
     * Sets the current tile the ship is at
     */
    void setCurrentTile( Tile3d *tile ) {
        m_CurrentTile = tile;
    }

    void render();
    void update();

    PSLaser *getLaser() { return m_Laser; }

    /**
     * Getters/Setters for the collision state flags
     */
    bool isInStation() const { return m_bInStation; }
    bool isInReactor() const { return m_bInReactor; }
    void setInStation( bool flag ) { m_bInStation = flag; }
    void setInReactor( bool flag ) { m_bInReactor = flag; }

}; // end Spaceship class