#include "SpaceshipCollisionResponse.h"
#include "VitalsHealth.h"
#include "Spaceship.h"
#include "Object.h"
using tlib::Object;
using tlib::OCVitalsHealth;

void SpaceshipCollisionResponse::respond()
{
    if( !getOwner()->isActive() ) return;

    IOCCollisionResponse::respond();
    OCVitalsHealth *cVitals = (OCVitalsHealth*)getOwner()->getComponent("vitals");
    cVitals->hit( 1 );

    // Check health status and deactivate shield
    if( cVitals->getHealth() == 2 )
        ((Spaceship*)getOwner())->getShield()->deactivate();
}