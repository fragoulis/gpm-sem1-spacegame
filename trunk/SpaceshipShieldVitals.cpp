#include "SpaceshipShieldVitals.h"
#include "ParticleSystemMgr.h"
#include "PSSmallExplosion.h"
#include "PSSmoke.h"

SpaceshipShieldVitals::SpaceshipShieldVitals( int iMaxLives, int iMaxHealth ):
OCVitalsLives( iMaxLives, iMaxHealth )
{}

void SpaceshipShieldVitals::onKill()
{
    // Create a particle explosion in its position
    //PSSmallExplosion *ex = 
    //    PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    //ex->start();

    //// Create a particle smoke in the same position
    //PSSmoke *smoke = 
    //    PSManager::Instance().addSmoke( getOwner()->getPos() );
    //smoke->start();
}

void SpaceshipShieldVitals::onRevive()
{
    
}