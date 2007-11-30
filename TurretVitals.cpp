#include "TurretVitals.h"
#include "Turret.h"
#include "ParticleSystemMgr.h"
#include "PSSmallExplosion.h"
#include "PSSmoke.h"
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
    PSSmallExplosion *ex = 
        PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->start();

    // Create a particle smoke in the same position
    PSSmoke *smoke = 
        PSManager::Instance().addSmoke( getOwner()->getPos() );
    smoke->start();

    // This object occupies a tile
    // We must set this tile's object pointer to null
    Tile3d* tile = Tilemap::Instance().getTile( getOwner()->getPos() );
    tile->setOccupant(0);
}

void TurretVitals::onRevive()
{}