#pragma once
#include <list>
#include "Object.h"
using namespace std;
using tlib::Object;

class Tile3d;
class Forcefield;
typedef list<Forcefield*> ForcefieldList;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class ForcefieldMgr : public Object
{
private:
    // The list of forcefieds
    static ForcefieldList m_vForcefields;

    // The warping effect step
    float m_fWarpStep;

public:
    /**
     * Initializes the object
     */
    void init();

    /**
     * Renders all the forcefields in the game
     */
    void render();

    /**
     * Updates the animations for all forcefields
     * Note: this has no overhead [except of course of the function call]
     * unless the animation is actually running
     */
    void update();

    /**
     * Pushes a forcefield to the list
     */
    static Forcefield* add( Tile3d *oTile );

private:
    /**
     * 
     */
    void remove( Forcefield *value );

}; // end of ForcefieldMgr class