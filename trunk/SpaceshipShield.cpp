#include "gl/glee.h"
#include "SpaceshipShield.h"
#include "Spaceship.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "CollisionDynamicBSphere.h"
#include "SpaceshipShieldCollisionResponse.h"
#include "SpaceshipShieldVitals.h"
#include "Clock.h"
#include "Timer.h"
#include "ShaderMgr.h"
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

    // Read animation time
    float fDuration;
    cfg.getFloat("anim_time", &fDuration);
    m_Timer->setDuration(fDuration);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color::black() );
    cMat->setDiffuse( Color::white() );
    setComponent( cMat );

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

    // Request a timer from the application clock
    m_Timer = Clock::Instance().GetTimer();
}

// ----------------------------------------------------------------------------
void SpaceshipShield::update() 
{
    setPos( m_oShip->getPos() );
}

// ----------------------------------------------------------------------------
void SpaceshipShield::render() const
{
    if( !m_Timer->isRunning() ) return;

    // User shield rendering program
    ShaderMgr::Instance().begin( ShaderMgr::HIT_GLOW );
    {   
        // Pass the collision point with the laser
        //glUniform3fv( ShaderMgr::Instance().getUniform("collPoint"), 
        //              3, m_vCollPoint.xyz() );

        // Pass a timer value to animation the effect
        glUniform1f( ShaderMgr::Instance().getUniform("timer"), 
                     (float)m_Timer->getElapsedTime() );
        
        // Enable blending
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        {
            // Draw the shield
            ((IOCVisual*)getComponent("visual"))->render();
        }
        glDisable( GL_BLEND );
    }
    ShaderMgr::Instance().end();

    //ShaderMgr::Instance().printProgramInfoLog(ShaderMgr::HIT_GLOW);
}

// ----------------------------------------------------------------------------
void SpaceshipShield::setCollPoint( const Vector3f& vCollPoint ) {
    m_Timer->restart();
    m_vCollPoint = vCollPoint;
}