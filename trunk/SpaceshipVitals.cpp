#include "SpaceshipVitals.h"
#include "ParticleSystemMgr.h"
#include "PSSmallExplosion.h"
#include "PSSmoke.h"
#include "Timer.h"
#include "Spaceship.h"

SpaceshipVitals::SpaceshipVitals( int iMaxLives, int iMaxHealth ):
OCVitalsLives( iMaxLives, iMaxHealth )
{}

void SpaceshipVitals::onKill()
{
    // Create a particle explosion in its position
    PSSmallExplosion *ex = 
        PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->start();
    ex = PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->start();
   
    // Create a particle smoke in the same position
    PSSmoke *smoke = PSManager::Instance().addSmoke( getOwner()->getPos() );
    smoke->start();
    smoke = PSManager::Instance().addSmoke( getOwner()->getPos() );
    smoke->start();
}

void SpaceshipVitals::onRevive()
{
    getOwner()->activate();
    ((Spaceship*)getOwner())->getShield()->activate();
    // (PSManager::Instance().getAssoc( getOwner() ))->stop();
}

void SpaceshipVitals::onEndOfLives()
{
    // Create a particle fire for dead spaceship
    PSSmallExplosion *ex = 
        PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->start();
    ex->getTimer()->setDuration(0.5);
    ex = PSManager::Instance().addSmallExplosion( getOwner()->getPos() );
    ex->getTimer()->setDuration(1.0);
    ex->start();
   
    PSSmoke *smoke = PSManager::Instance().addSmoke( getOwner()->getPos() );
    smoke->start();
    smoke->getTimer()->setDuration(2.0);
    smoke = PSManager::Instance().addSmoke( getOwner()->getPos() );
    smoke->getTimer()->setDuration(4.0);
    smoke->start();
}