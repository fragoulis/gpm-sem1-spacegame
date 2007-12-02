#include "SpaceshipShieldCollisionResponse.h"
#include "VitalsHealth.h"
#include "Object.h"
using tlib::Object;
using tlib::OCVitalsHealth;

void SpaceshipShieldCollisionResponse::respond( const Vector3f &vCollVector ) const
{
    IOCCollisionResponse::respond( vCollVector );
    OCVitalsHealth *cVitals = (OCVitalsHealth*)getOwner()->getComponent("vitals");
    cVitals->hit( 1 );
}