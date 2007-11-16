#pragma once
#include <vector>
#include "Object.h"
using namespace std;
using tlib::Object;

class Turret;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class TurretMgr
{
private:
    // The list of forcefieds
    static vector<Turret*> m_vTurrets;

public:
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

    void destroy();

    /**
     * Pushes a turret to the list
     */
    static void add( Turret *tr ) {
        m_vTurrets.push_back( tr );
    }

    /**
     * Returns a turret by its index
     */
    Turret* getTurret( int index ) {
        return m_vTurrets[index];
    }

}; // end of TurretMgr class