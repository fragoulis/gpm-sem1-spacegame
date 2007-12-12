#include "ReactorVitals.h"
#include "Reactor.h"
#include "EventMgr.h"
#include "FinalExplosion.h"
#include "Timer.h"
#include "Config.h"
#include "ObjectMgr.h"
#include "Autopilot.h"
using tlib::Config;

ReactorVitals::ReactorVitals( int iMaxHealth ):
OCVitalsHealth( iMaxHealth )
{
	Config cfg("config.txt");
	cfg.loadBlock("reactor");
	cfg.getDouble("secs_to_explode", &m_dSecsToFire);
}

void ReactorVitals::onKill()
{
	// Start the glowing sphere
    ((Reactor*)getOwner())->getGlowTimer()->start();

	// Add the explosion to the event manager
	FinalExplosion *ex = new FinalExplosion( 
		getOwner()->getPos(), 
		m_dSecsToFire 
		);
	EventMgr::Instance().addEvent( dynamic_cast<Event*>(ex) );

    // Deactivate spaceship
    ObjectMgr::Instance().getShip().deactivate();

    // Start Autopilot
    Autopilot::Instance().start();
}

void ReactorVitals::onRevive()
{}