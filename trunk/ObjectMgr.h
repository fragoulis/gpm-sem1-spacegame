#pragma once
#include <list>
using std::list;
#include "Singleton.h"

// Forward declarations
class Tile3d;
namespace tlib { class Object; }
using tlib::Object;

typedef list<Object*> ObjectList;

class ObjectMgr : public Singleton<ObjectMgr>
{
    friend Singleton<ObjectMgr>;

private:
    // The list of particle systems
    ObjectList m_vObjects;

public:
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