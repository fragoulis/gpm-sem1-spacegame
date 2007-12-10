#include "TurretVitals.h"
#include "Turret.h"
#include "ParticleSystemMgr.h"
#include "PSCommon.h"
#include "Tilemap.h"

TurretVitals::TurretVitals( int iMaxHealth ):
OCVitalsHealth( iMaxHealth )
{}

void TurretVitals::onKill()
{
    // When a turret is killed, delete its laser system
    PSLaser *laser = ((Turret*)getOwner())->getLaser();
    PSManager::Instance().remove( (ParticleSystem*)laser );

    // Create a particle explosion in its position
    (PSManager::Instance().addSystem( PSManager::EXPLOSION, getOwner()->getPos() ))->start();
    (PSManager::Instance().addSystem( PSManager::SMOKE, getOwner()->getPos() ))->start();

    // This object occupies a tile
    // We must set this tile's object pointer to null
    Tile3d* tile = Tilemap::Instance().getTile( getOwner()->getPos() );
    tile->setOccupant(0);
}

void TurretVitals::onRevive()
{}