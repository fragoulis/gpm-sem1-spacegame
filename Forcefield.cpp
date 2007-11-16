#include "Forcefield.h"
#include "ForcefieldAnimation.h"
#include "CollisionBBox.h"
#include "SimpleMaterial.h"
#include "Config.h"
using tlib::OCCollisionBBox;
using tlib::OCSimpleMaterial;
using tlib::Config;

Forcefield::Forcefield()
{
    // Read the bouding box for the forcefield and 
    // pass it to the parent constructor
    Config cfg("config.txt");
    cfg.loadBlock("forcefield");

    // Read to transparent factor
    cfg.getFloat("to_trans", &m_fToTransparentFactor);

    // Read bounding box for the forcefield
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // Set the forcefield's bounding box
    // We are dividing by two because all of our bounding 
    // boxes are expressed in half sized dimensions
    OCCollisionBBox *cColBox = (OCCollisionBBox*)getComponent("collision");
    cColBox->setBBox( Vector3f( vfBBox ) * 0.5f );

    // Read forcefield's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Initialize material component
    setComponent( new OCSimpleMaterial( 
        Color::black(),
        Color(vfColor),
        Color::null()) 
        );

    // Initialize animation component
    setComponent( new ForcefieldAnimation );
}