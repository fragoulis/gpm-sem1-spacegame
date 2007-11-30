#pragma once
#include <list>
#include "Object.h"
using namespace std;

class RotatingBlade;
typedef list<RotatingBlade*> RotatingBladeList;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class BladeMgr
{
private:
    // The list of blades in our scene
    static RotatingBladeList m_vBlades;

public:
    /**
     * Renders all blades in the game
     */
    void render();

    /**
     * Updates the animations for all blades
     * Note: this has no overhead [except of course of the function call]
     * unless the animation is actually running
     */
    void update();

    /**
     * Pushes a blade to the list
     */
    static void add( RotatingBlade *obj ) {
        m_vBlades.push_back( obj );
    }

private:
    /**
     * 
     */
    void remove( RotatingBlade *value );

}; // end of BladeMgr class