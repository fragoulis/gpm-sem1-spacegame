#pragma once
#include <vector>
#include "Object.h"
using namespace std;
using tlib::Object;

class Outlet;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class OutletMgr : public Object
{
private:
    static vector<Outlet*> m_vOutlets;

public:
    /**
     * Initializes the object
     */
    void init();

    /**
     * Renders all the Outlets in the game
     */
    void render();

    /**
     * Updates the animations for all Outlets
     * Note: this has no overhead [except of course of the function call]
     * unless the animation is actually running
     */
    void update();

    /**
     * Pushes an outlet to the list
     */
    static void add( Outlet *ol ) {
        m_vOutlets.push_back( ol );
    }

    /**
     * Returns an outlet by its index
     */
    Outlet* getOutlet( int index ) {
        return m_vOutlets[index];
    }

}; // end of DoorRenderer class