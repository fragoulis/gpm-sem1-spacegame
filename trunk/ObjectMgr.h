#pragma once
#include <list>
using std::list;
#include "Singleton.h"
#include "Spaceship.h"
#include "SpaceshipShield.h"
#include "Reactor.h"
#include "Spacestation.h"
#include "SpacestationCorridors.h"
#include "Spacecube.h"
// Object managers
#include "DoorMgr.h"
#include "BladeMgr.h"
#include "ForcefieldMgr.h"
#include "OutletMgr.h"
#include "TurretMgr.h"

// Forward declarations
class Tile3d;
typedef list<Object*> ObjectList;

class ObjectMgr : public Singleton<ObjectMgr>
{
    friend Singleton<ObjectMgr>;

private:
    // This list holds all obejct's in the scene
    // except of the reactor, the shield and the 
    // spaceship which are unique
    //ObjectList m_vObjects;

    // An imaginary radius of a sphere, used to skip
    // object outside of it
    float m_fCullDistance;

    // Unique object's of our world
    Spaceship               m_Ship;
    SpaceshipShield         m_Shield;
    Reactor                 m_Reactor;
    Spacestation            m_Station;
    SpacestationCorridors   m_Corridors;
    Spacecube               m_Spacecube;

    // These are managers for the barriers, the power outlets 
    // and the defence guns
    DoorMgr         m_DoorMgr;
    BladeMgr        m_BladeMgr;
    ForcefieldMgr   m_ForcefieldMgr;
    OutletMgr       m_OutletMgr;
    TurretMgr       m_TurretMgr;

    // Flag for whether we should render the spaceship
    bool m_bDrawSpaceship;

public:
    /**
     * Accessors for the unique objects
     */
    const Spaceship& getShip() const { return m_Ship; }
    Spaceship& getShip() { return m_Ship; }
    const SpaceshipShield& getShield() const { return m_Shield; }
    const Reactor& getReactor() const { return m_Reactor; }
    const SpacestationCorridors& getCorridors() const { return m_Corridors; }
    
    void showShip() { m_bDrawSpaceship = true; }
    void hideShip() { m_bDrawSpaceship = false; }
    bool isShipHidden() const { return !m_bDrawSpaceship; }

    /**
     * 
     */
    void update();

    /**
     * 
     */
    void render();
    void renderForMinimap( const float fSizeRatio ) const;

    /**
     * 
     */
    void checkCollision();

    /**
     * 
     */
    void addLight( Tile3d *oTile ) const;
    void addDoor( Tile3d *oTile ) const;
    void addBlade( Tile3d *oTile ) const;
    void addForcefield( Tile3d *oTile ) const;
    void addOutlet( Tile3d *oTile ) const;
    void addTurret( Tile3d *oTile ) const;

    /**
     * Checks if an object is far from the currently active
     * camera so that it can be excused from the updates
     * and the rendering
     */
    bool isCulled( Object *obj ) const;

    /** 
     * Calls the initialization functions for the unique objects
     */
    void init();

private:
    /**
     * Constructor
     */
    ObjectMgr();


    /**
     * Destructor
     */
    ~ObjectMgr();

}; // end of ObjecMgr class