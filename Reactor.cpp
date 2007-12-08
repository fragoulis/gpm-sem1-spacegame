#include "gl/glee.h"
#include "Reactor.h"
#include "Logger.h"
#include "Config.h"
#include "Texture.h"
#include "SimpleMaterial.h"
#include "SingleTexture.h"
#include "VisualVertexArraySphere.h"
#include "CollisionBSphere.h"
#include "ReactorCollisionResponse.h"
#include "ReactorVitals.h"
#include "ShaderMgr.h"
#include "Clock.h"
#include "Timer.h"

using namespace tlib;

Reactor::Reactor() {}

void Reactor::init()
{
    _LOG("Setting up reactor...");

    // Set object's type
    setType( REACTOR );

    m_AnimTimer = Clock::Instance().GetTimer();
    m_AnimTimer->start();
    m_GlowTimer = Clock::Instance().GetTimer();
    m_GlowTimer->setDuration(1.0);
    m_GlowTimer->setScale(0.2f);

    Config cfg("config.txt");
    cfg.loadBlock("reactor");

    // Initialize position
    float vfPos[3];
    cfg.getFloat("init_pos", vfPos, 3);
    getPos().xyz( vfPos );

    // Initialize member variables
    cfg.getFloat("rot_factor", &m_fRotFactor );

    // Read part's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read part's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read part's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Read reactor's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setDiffuse( Color( vfColor ) );
    cMat->setSpecular( Color( 0.6f, 0.6f, 0.6f, 1.0f ) );
    cMat->setShininess( 60.0f );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iStacks, iSlices ) );

    // Get panel's texture
    string sTexture;
    cfg.getString("texture", sTexture );

    // Initialize texture component
    setComponent( new OCSingleTexture( sTexture.c_str() ) );

    // Initialize collision component
    setComponent( new OCCollisionBSphere( fRadius ) );

    // Initialize collision response component
    setComponent( new ReactorCollisionResponse );

    // Read health
    int iHealth;
    cfg.getInt("hits", &iHealth);

    // Initialize health component
    setComponent( new ReactorVitals( iHealth ) );

} // end setup()

// ----------------------------------------------------------------------------
void Reactor::render()
{ 
    ShaderMgr::Instance().begin( ShaderMgr::POINT_AND_SPOT_SINGLE_MOV_TEX );
    {
        glUniform1f( ShaderMgr::Instance().getUniform("timer"), 
                     (float)m_AnimTimer->getElapsedTime() );

        glUniform1f( ShaderMgr::Instance().getUniform("glow_timer"), 
                     (float)m_GlowTimer->getElapsedTime() );

        ((IOCVisual*)getComponent("visual"))->render();
    }
    ShaderMgr::Instance().end();    
}

// ----------------------------------------------------------------------------
void Reactor::update()
{
    // Update and check health status
	((IOCVitals*)getComponent("vitals"))->update();
}