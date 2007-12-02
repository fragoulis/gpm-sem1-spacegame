#include "SpaceshipShield.h"
#include "Spaceship.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "CollisionDynamicBSphere.h"
#include "SpaceshipShieldCollisionResponse.h"
#include "SpaceshipShieldVitals.h"
#include "Config.h"
using namespace tlib;

// ----------------------------------------------------------------------------
void SpaceshipShield::init( Spaceship *oShip )
{
    // Set object's type
    setType( SPACESHIP_SHIELD );

    // Assign the spaceship pointer
    m_oShip = oShip;

    Config cfg("config.txt");
    cfg.loadBlock("shield");

    // Initialize material component
    setComponent( new OCSimpleMaterial(
        Color::black(),
        Color(1.0f,1.0f,1.0f,0.3f),
        Color::null()) );

    // Read maximum lives and health
    int iMaxLives, iMaxHealth;
    cfg.getInt("lives", &iMaxLives);
    cfg.getInt("hits", &iMaxHealth);

    // Read shield's life

    // Initialize health component
    setComponent( new SpaceshipShieldVitals( iMaxLives, iMaxHealth ) );

    // Read shield's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read shield's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read shield's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iStacks, iSlices ) );

    // Initialize collision component
    setComponent( new OCCollisionDynamicBSphere( fRadius ) );

    // Initialize collision response component
    setComponent( new SpaceshipShieldCollisionResponse );
}

// ----------------------------------------------------------------------------
void SpaceshipShield::update() 
{
    m_vPos = m_oShip->getPos();
}