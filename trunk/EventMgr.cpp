#include "EventMgr.h"
#include "Event.h"
#include "Timer.h"
#include "Logger.h"
using tlib::Logger;

EventMgr::EventMgr()
{}

EventMgr::~EventMgr()
{
    _LOG("Deleting events");
	EventList::iterator iter;
	for( iter = m_Events.begin(); 
         iter != m_Events.end(); 
         ++iter )
    {
        delete *iter;
		*iter = 0;
    }
}

void EventMgr::update() const
{
    EventList::const_iterator iter;
	for( iter = m_Events.begin(); 
         iter != m_Events.end(); 
         ++iter )
    {
		Event *e = *iter;
		if( !e->isFired() )
		{
			if( !e->getTimer()->isRunning() )
			{
				e->fire();
				e->fired();
			}
		}
    }
}