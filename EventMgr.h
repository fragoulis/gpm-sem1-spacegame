#pragma once
#include "Singleton.h"
#include <vector>
using std::vector;

class Event;
class EventMgr : public Singleton<EventMgr>
{
	friend Singleton<EventMgr>;
private:
	typedef vector<Event*> EventList;
	EventList m_Events;

public:
	inline void addEvent( Event* );
	void update() const;

private:
	EventMgr();
	~EventMgr();
};

void EventMgr::addEvent( Event *e ) {
	m_Events.push_back(e);
}