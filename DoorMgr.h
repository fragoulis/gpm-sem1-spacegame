#pragma once
#include <vector>
#include "Object.h"
using namespace std;
using tlib::Object;

class Door;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class DoorMgr : public Object
{
private:
    static vector<Door*> m_vDoors;

public:
    /**
     * Initializes the object
     */
    void init();

    /**
     * Renders all the doors in the game
     */
    void render();

    /**
     * Updates the animations for all doors
     * Note: this has no overhead [except of course of the function call]
     * unless the animation is actually running
     */
    void update();

    /**
     * Pushes a door to the list
     */
    static void add( Door *door ) {
        m_vDoors.push_back( door );
    }

    /**
     * Returns a door by its index
     */
    Door* getDoor( int index ) {
        return m_vDoors[index];
    }

}; // end of DoorRenderer class