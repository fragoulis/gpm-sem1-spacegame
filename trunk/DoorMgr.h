#pragma once
#include <list>
#include "Object.h"
using namespace std;
using tlib::Object;

class Tile3d;
class Door;
typedef list<Door*> DoorList;

/** 
 * This class should actually be a singleton but there would 
 * be implications given the fact that it already inherits from 
 * Object.
 * To get around this we declare the date we care for as static
 */
class DoorMgr : public Object
{
private:
    static DoorList m_vDoors;

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

    static Door* add( Tile3d *oTile );

}; // end of DoorMgr class