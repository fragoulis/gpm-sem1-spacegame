#include "gl/glee.h"
#include "SpaceshipShield.h"
#include "Spaceship.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "CollisionDynamicBSphere.h"
#include "SpaceshipShieldCollisionResponse.h"
//#include "SpaceshipShieldVitals.h"
#include "Clock.h"
#include "Timer.h"
#include "ShaderMgr.h"
#include "Config.h"
#include "MultiTexture.h"
using namespace tlib;

// ----------------------------------------------------------------------------
void SpaceshipShield::init( Spaceship *oShip )
{
    // Set object's type
    setType( SPACESHIP_SHIELD );

    // Assign the spaceship pointer
    m_oShip = oShip;
    m_oShip->setShield( this );

    Config cfg("config.txt");
    cfg.loadBlock("shield");

    // Read animation time
    float fNoiseDuration, fGlowDuration;
    cfg.getFloat("noise_time", &fNoiseDuration);
    cfg.getFloat("glow_time", &fGlowDuration);

    // Request a timer from the application clock
    m_NoiseTimer = Clock::Instance().GetTimer();
    m_NoiseTimer->setDuration(fNoiseDuration);
    m_NoiseTimer->start();

    m_GlowTimer = Clock::Instance().GetTimer();
    m_GlowTimer->setDuration(fGlowDuration);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color::black() );
    cMat->setDiffuse( Color::white() );
    setComponent( cMat );

    // Read shield's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read shield's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read shield's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Read textures
    string sColorMap, sNoiseMap;
    cfg.getString("color_map", sColorMap);
    cfg.getString("noise_map", sNoiseMap);

    OCMultiTexture *cTex = new OCMultiTexture(2);
    cTex->set(0, sColorMap.c_str());
    cTex->setName(0, "colorMap");
    cTex->set(1, sNoiseMap.c_str());
    cTex->setName(1, "noiseMap");
    setComponent( cTex );

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
    setPos( m_oShip->getPos() );
}

// ----------------------------------------------------------------------------
void SpaceshipShield::render() const
{
    if( !isActive() ) return;

    // User shield rendering program
    ShaderMgr::Instance().begin( ShaderMgr::HIT_GLOW );
    {   
        ((IOCTexture*)getComponent("texture"))->apply();

        glUniform1f( ShaderMgr::Instance().getUniform("noise_timer"), 
                     (float)m_NoiseTimer->getElapsedTime() );

        float glowTimer = 1.0f;
        if( m_GlowTimer->isRunning() ) {
            glowTimer = (float)m_GlowTimer->getElapsedTime();
        }
        glUniform1f( ShaderMgr::Instance().getUniform("glow_timer"), glowTimer );
        
        // Enable blending
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        {
            // Draw the shield
            ((IOCVisual*)getComponent("visual"))->render();
        }
        glDisable( GL_BLEND );

        ((IOCTexture*)getComponent("texture"))->reset();
    }
    ShaderMgr::Instance().end();

    //ShaderMgr::Instance().printProgramInfoLog(ShaderMgr::HIT_GLOW);
}

// ----------------------------------------------------------------------------
void SpaceshipShield::setCollPoint( const Vector3f& vCollPoint ) {
    m_GlowTimer->restart();
    m_vCollPoint = vCollPoint;
}