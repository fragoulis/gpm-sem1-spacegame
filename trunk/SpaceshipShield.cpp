#include "SpaceshipShield.h"
#include "Spaceship.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "CollisionDynamicBSphere.h"
//#include "ShieldCollisionResponse.h"
#include "SpaceshipCollisionResponse.h"
#include "Config.h"
using namespace tlib;

// ----------------------------------------------------------------------------
void SpaceshipShield::init( Spaceship *oShip )
{
    // Assign the spaceship pointer
    m_oShip = oShip;

    Config cfg("config.txt");
    cfg.loadBlock("shield");

    // Read shield's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read shield's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read shield's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Initialize material component
    setComponent( new OCSimpleMaterial(
        Color::black(),
        Color(1.0f,1.0f,1.0f,0.3f),
        Color::null()) );

    //setComponent( new OCSimpleMaterial );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iStacks, iSlices ) );

    // Initialize collision component
    setComponent( new OCCollisionDynamicBSphere( fRadius ) );

    // Initialize collision response component
    setComponent( new SpaceshipCollisionResponse );
}

// ----------------------------------------------------------------------------
void SpaceshipShield::update()
{
    //m_vPos = m_oShip->getPos() + Vector3f(0.55f,0.0f,0.0f);
    m_vPos = m_oShip->getPos();
}