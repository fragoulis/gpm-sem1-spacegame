#include "Event.h"
#include "Clock.h"
#include "Timer.h"

Event::Event( double dToFire ):
m_bIsFired(false)
{
	m_Timer = Clock::Instance().GetTimer();
	m_Timer->setDuration(dToFire);
	m_Timer->start();
}