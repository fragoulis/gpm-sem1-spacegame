#pragma once
#include <list>
using std::list;
#include "Singleton.h"
#include "Spaceship.h"
#include "SpaceshipShield.h"
#include "Reactor.h"
#include "Spacestation.h"
#include "SpacestationCorridors.h"

// Forward declarations
class Tile3d;

namespace tlib
{

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

    public:
        /**
         * Accessors for the unique objects
         */
        const Spaceship& getShip() const { return m_Ship; }
        Spaceship& getShip() { return m_Ship; }
        const SpaceshipShield& getShield() const { return m_Shield; }
        const Reactor& getReactor() const { return m_Reactor; }

        /**
         * 
         */
        void update();

        /**
         * 
         */
        void render();

        /**
         * 
         */
        void checkCollision();

        /**
         * 
         */
        void addDoor( Tile3d *oTile );
        
        /**
         * 
         */
        void addBlade( Tile3d *oTile );

        /**
         * 
         */
        void addForcefield( Tile3d *oTile );

        /**
         * 
         */
        void addOutlet( Tile3d *oTile );

        /**
         * 
         */
        void addTurret( Tile3d *oTile );

        /**
         * Checks if an object is far from the currently active
         * camera so that it can be excused from the updates
         * and the rendering
         */
        bool isCulled( Object *obj );

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

}; // end of ParticleSystemMgr class

} // end of namespace tlib