#include "ReactorCollisionResponse.h"
#include "VitalsHealth.h"
#include "Object.h"
using tlib::Object;
using tlib::OCVitalsHealth;

void ReactorCollisionResponse::respond()
{
    OCVitalsHealth *cVitals = (OCVitalsHealth*)getOwner()->getComponent("vitals");
    cVitals->hit( 1 );
}