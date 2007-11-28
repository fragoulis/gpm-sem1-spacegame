#include "TurretVitals.h"
#include "Turret.h"
#include "ParticleSystemMgr.h"
#include "PSSmallExplosion.h"
#include "ParticleEmitter.h"

TurretVitals::TurretVitals( int iMaxHealth ):
OCVitalsHealth( iMaxHealth )
{}

void TurretVitals::onKill()
{
    // When a turret is killed, delete its laser system
    PSLaser *laser = ((Turret*)getOwner())->getLaser();
    PSManager::Instance().remove( (ParticleSystem*)laser );

    // Create a particle explosion in its position
    PSSmallExplosion *ex = 
        PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->start();
}

void TurretVitals::onRevive()
{}