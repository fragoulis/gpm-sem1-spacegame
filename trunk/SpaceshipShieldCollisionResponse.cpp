#include "SpaceshipShieldCollisionResponse.h"
#include "SpaceshipShield.h"
#include "VitalsHealth.h"
#include "Object.h"
using tlib::Object;
using tlib::OCVitalsHealth;

void SpaceshipShieldCollisionResponse::respond() const
{
    //std::cout << "Center: " << getOwner()->getPos() << std::endl;
    //std::cout << "Collision point: " << getCollPoint() << std::endl;
    ((SpaceshipShield*)getOwner())->setCollPoint( getCollPoint() );

    OCVitalsHealth *cVitals = (OCVitalsHealth*)getOwner()->getComponent("vitals");
    cVitals->hit( 1 );
}