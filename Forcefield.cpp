#include "Forcefield.h"
#include "ForcefieldAnimation.h"
#include "CollisionBBox.h"
#include "Config.h"
using namespace tlib;

Forcefield::Forcefield():
m_fAlpha(1.0f)
{
    // Read the bouding box for the forcefield and 
    // pass it to the parent constructor
    Config cfg("config.txt");
    cfg.loadBlock("forcefield");

    // Read to transparent factor
    cfg.getFloat("anim_time", &m_fToTransparentFactor);

    // Read the bouding box of the forcefield
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // Set the forcefield's bounding box
    // We are dividing by two because all of our bounding 
    // boxes are expressed in half sized dimensions
    OCCollisionBBox *cColBox = (OCCollisionBBox*)getComponent("collision");
    cColBox->setBBox( Vector3f( vfBBox ) * 0.5f );

    // Initialize animation component
    setComponent( new ForcefieldAnimation );
}