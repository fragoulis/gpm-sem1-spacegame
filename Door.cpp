#include "Door.h"
#include "DoorAnimation.h"
#include "CollisionBBox.h"
#include "Config.h"
using tlib::OCCollisionBBox;
using tlib::Config;

Vector3f Door::PanelDim;

Door::Door()
{
    // Read the bouding box for the door and 
    // pass it to the parent constructor
    Config cfg("config.txt");
    cfg.loadBlock("door");

    // Read bounding box for the door
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // Set the door's bounding box
    // We are dividing by two because all of our bounding 
    // boxes are expressed in half sized dimensions
    OCCollisionBBox *cColBox = (OCCollisionBBox*)getComponent("collision");
    cColBox->setBBox( Vector3f( vfBBox ) * 0.5f );

    // Initialize the panels
    m_Panel[DoorPanel::Left].init( DoorPanel::Left );
    m_Panel[DoorPanel::Right].init( DoorPanel::Right );
    m_Panel[DoorPanel::Top].init( DoorPanel::Top );
    m_Panel[DoorPanel::Bottom].init( DoorPanel::Bottom );

    // Read panel half dimensions
    cfg.loadBlock("door_panel");
    float vfDim[3];
    cfg.getFloat("half_dim", vfDim, 3);
    PanelDim.xyz( vfDim );

    // Initialize animation component
    setComponent( new DoorAnimation );
}