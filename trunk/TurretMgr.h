#pragma once
#include <list>
#include "Object.h"
using namespace std;
using tlib::Object;

class Tile3d;
class Turret;
typedef list<Turret*> TurretList;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class TurretMgr : public Object
{
private:
    // The list of forcefieds
    static TurretList m_vTurrets;

public:
    /**
     * Initializes the object
     * Note: must be called after spaceship and corridors 
     *       have been initialized
     */
    void init( Object *obj );

    /**
     * Renders all the turrets in the game
     */
    void render();

    /**
     * Updates the animations for all turrets
     * Note: this has no overhead [except of course of the function call]
     * unless the animation is actually running
     */
    void update();

    static Turret* add( Tile3d *oTile );

private:
    /**
     * 
     */
    void remove( Turret *turret );

}; // end of TurretMgr class