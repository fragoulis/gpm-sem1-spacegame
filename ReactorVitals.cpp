#include "ReactorVitals.h"
#include "Reactor.h"
#include "Timer.h"

ReactorVitals::ReactorVitals( int iMaxHealth ):
OCVitalsHealth( iMaxHealth )
{}

void ReactorVitals::onKill()
{
    ((Reactor*)getOwner())->getGlowTimer()->start();
}

void ReactorVitals::onRevive()
{}