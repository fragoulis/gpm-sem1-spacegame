#include "SpaceshipShieldCollisionResponse.h"
#include "SpaceshipShield.h"
#include "VitalsHealth.h"
#include "ParticleSystemMgr.h"
#include "PSSmoke.h"
#include "Timer.h"
#include "Object.h"
using tlib::Object;
using tlib::OCVitalsHealth;

void SpaceshipShieldCollisionResponse::respond()
{
    //std::cout << "Center: " << getOwner()->getPos() << std::endl;
    //std::cout << "Collision point: " << getCollPoint() << std::endl;
    ((SpaceshipShield*)getOwner())->setCollPoint( getCollPoint() );

    // Hit the spaceship [even though this is the shields response 
    // to the lasers]
    Object *oObj = ((SpaceshipShield*)getOwner())->getShip();
    OCVitalsHealth *cVitals = (OCVitalsHealth*)oObj->getComponent("vitals");
    cVitals->hit( 1 );

    // Here it would be to have a particle smoke effect on the spaceship 
    // after its suffered some damage
    //if( cVitals->healthRatio() > 0.49f && cVitals->healthRatio() < 0.51f )
    //{
    //    // Create a particle smoke in the same position
    //    PSSmoke *smoke = 
    //        PSManager::Instance().addSmoke( getOwner()->getPos() );
    //    smoke->getTimer()->setDuration(0.0);
    //    smoke->start();
    //    //PSManager::Instance().assoc( getOwner(), smoke );
    //}

    // Check health status and deactivate shield
    if( cVitals->getHealth() == 2 )
        getOwner()->deactivate();
}