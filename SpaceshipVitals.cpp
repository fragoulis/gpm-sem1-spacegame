#include "SpaceshipVitals.h"
#include "ParticleSystemMgr.h"
#include "PSCommon.h"
#include "Timer.h"
#include "Spaceship.h"

SpaceshipVitals::SpaceshipVitals( int iMaxLives, int iMaxHealth ):
OCVitalsLives( iMaxLives, iMaxHealth )
{}

void SpaceshipVitals::onKill()
{
    // Create a particle explosion in its position
    (PSManager::Instance().addSystem( PSManager::EXPLOSION, getOwner()->getPos() ))->start();
    (PSManager::Instance().addSystem( PSManager::EXPLOSION, getOwner()->getPos() ))->start();
    
    // Create a particle smoke in the same position
    (PSManager::Instance().addSystem( PSManager::SMOKE, getOwner()->getPos() ))->start();
    (PSManager::Instance().addSystem( PSManager::SMOKE, getOwner()->getPos() ))->start();
}

void SpaceshipVitals::onRevive()
{
    getOwner()->activate();
    ((Spaceship*)getOwner())->getShield()->activate();
    // (PSManager::Instance().getAssoc( getOwner() ))->stop();
}

void SpaceshipVitals::onEndOfLives()
{
    // Create a particle fire and smoke for dead spaceship
    PSCommon *ex;
    
    ex = PSManager::Instance().addSystem( PSManager::EXPLOSION, getOwner()->getPos() );
    ex->getTimer()->setDuration(0.5);
    ex->start();
    
    ex = PSManager::Instance().addSystem( PSManager::EXPLOSION, getOwner()->getPos() );
    ex->getTimer()->setDuration(1.0);
    ex->start();
   
    ex = PSManager::Instance().addSystem( PSManager::SMOKE, getOwner()->getPos() );
    ex->getTimer()->setDuration(2.0);
    ex->start();
    
    ex = PSManager::Instance().addSystem( PSManager::SMOKE, getOwner()->getPos() );
    ex->getTimer()->setDuration(4.0);
    ex->start();
}