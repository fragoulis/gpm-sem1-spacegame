#include "Turret.h"
#include "CollisionBSphere.h"
#include "QuatRotation.h"
#include "TurretVitals.h"
#include "TurretCollisionResponse.h"
#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "Config.h"

using namespace tlib;

void Turret::init()
{
    Config cfg("config.txt");
    cfg.loadBlock("turret");

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

    // Initialize collision response component
    setComponent( new TurretCollisionResponse );

    // Read laser color
    float vfLaserColor[4];
    cfg.getFloat("laser_color", vfLaserColor, 4);

    // Read laser emitter offset
    float fEmitterOffset;
    cfg.getFloat("laser_offset", &fEmitterOffset);

    // A small [necessary] correction to the laser emitter position
    float vfCorrect[3];
    cfg.getFloat("correct_laser", vfCorrect, 3);
    Vector3f vNewPos = m_vPos;
    vNewPos.add( vfCorrect );

    // Read total hits a turret can withstand
    int iHits;
    cfg.getInt("hits", &iHits);

    // Initialize health component
    setComponent( new TurretVitals( iHits ) );

    // Initialize laser system
    m_Laser = PSManager::Instance().addLaser( 
        this, 
        vNewPos, 
        fEmitterOffset, 
        vfLaserColor );
}