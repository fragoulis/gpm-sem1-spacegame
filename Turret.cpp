#include "Turret.h"
#include "VisualGXModel.h"
#include "CollisionBSphere.h"
#include "QuatRotation.h"
#include "Config.h"

using namespace tlib;

Turret::Turret()
{
    Config cfg("config.txt");
    cfg.loadBlock("turret");

    // Initialize visual component
    string sModel;
    float fScale;
    cfg.getString("model", sModel );
    cfg.getFloat("scale", &fScale );
    setComponent( new OCGXModel( sModel.c_str(), fScale ) );

    // Initialize the rotation component
    OCQuatRotation *cRot = new OCQuatRotation;
    cRot->setView  ( Vector3f::Front() );
    cRot->setUp    ( Vector3f::Up() );
    cRot->setRight ( Vector3f::Right() );
    setComponent( cRot );

    // Read bounding sphere's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Initialize collision component
    setComponent( new OCCollisionBSphere( fRadius ) );
}