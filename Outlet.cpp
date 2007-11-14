#include "Outlet.h"
#include "CollisionBBox.h"
#include "OutletAnimation.h"
#include "SimpleMaterial.h"
#include "OutletCollisionResponse.h"
#include "Config.h"

using namespace tlib;

Outlet::Outlet(): m_pBarrier(0)
{
    Config cfg("config.txt");
    cfg.loadBlock("outlet");

    // Read bounding box from configuration file
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // Correct outlet's position by rotating it 90 degrees
    // about the Y-axis
    m_qDir.toRotation( (float)M_PI_2, Vector3f::Up() );

    // Initialize collision component
    setComponent( new OCCollisionBBox( Vector3f( vfBBox ) ) );

    // Initialize animation component
    setComponent( new OutletAnimation );

    // Initialize material component
    //setComponent( new OCSimpleMaterial(Color::black(),Color::red(),Color::null()) );
    setComponent( new OCSimpleMaterial );

    // Initialize collision response component
    setComponent( new OutletCollisionResponse );
}