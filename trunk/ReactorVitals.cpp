#include "ReactorVitals.h"
#include "Reactor.h"
#include "EventMgr.h"
#include "EFinalExplosion.h"
#include "EPanCamera.h"
#include "Timer.h"
#include "Animation.h"
#include "Config.h"
#include "Autopilot.h"
#include "Tilemap.h"
#include "CameraMgr.h"
#include "ObjectMgr.h"
using tlib::Config;

ReactorVitals::ReactorVitals( int iMaxHealth ):
OCVitalsHealth( iMaxHealth )
{
	Config cfg("config.txt");
	cfg.loadBlock("reactor");
	cfg.getDouble("secs_to_explode", &m_dSecToFire);
    cfg.getDouble("sec_to_pancam", &m_dSecToPanCam);
}

// ----------------------------------------------------------------------------
void ReactorVitals::onKill()
{
	// Start the glowing sphere
    ((Reactor*)getOwner())->getGlowTimer()->start();

	// Add the explosion to the event manager
	EFinalExplosion *ex = new EFinalExplosion( 
		getOwner()->getPos(), 
		m_dSecToFire 
		);
	EventMgr::Instance().addEvent( dynamic_cast<Event*>(ex) );

    // Start Autopilot
    Autopilot::Instance().start();

    // Open the escape door
    Object *oDoor = Tilemap::Instance().getExitDoorTile()->getOccupant();
    ((IOCAnimation*)oDoor->getComponent("animation"))->start();

    // Add an event for a camera change
    EPanCamera *pc = new EPanCamera( m_dSecToPanCam );
    EventMgr::Instance().addEvent( dynamic_cast<Event*>(pc) );

    CameraMgr::Instance().activate("stalker");
    ObjectMgr::Instance().showShip();
}

void ReactorVitals::onRevive()
{}