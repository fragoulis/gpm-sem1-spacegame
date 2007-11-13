#include "Door.h"
#include "DoorAnimation.h"
#include "Config.h"
using tlib::Config;

Door::Door()
{
    // Read the bouding box for the door and 
    // pass it to the parent constructor
    Config cfg("config.txt");
    cfg.loadBlock("door");
    
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // We are dividing by two because all of our bounding 
    // boxes are expressed in half sized dimensions
    Barrier::Barrier( Vector3f( vfBBox ) * 0.5f );

    // Initialize the panels
    m_Panel[DoorPanel::Left].init( DoorPanel::Left );
    m_Panel[DoorPanel::Right].init( DoorPanel::Right );
    m_Panel[DoorPanel::Top].init( DoorPanel::Top );
    m_Panel[DoorPanel::Bottom].init( DoorPanel::Bottom );

    // Initialize animation component
    setComponent( new DoorAnimation );
}