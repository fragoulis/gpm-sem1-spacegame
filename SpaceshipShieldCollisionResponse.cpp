#include "SpaceshipShieldCollisionResponse.h"
#include "SpaceshipShield.h"
#include "VitalsHealth.h"
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

    // Check health status and deactivate shield
    if( cVitals->getHealth() == 2 )
        getOwner()->deactivate();
}