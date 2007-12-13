#pragma once
#include "Event.h"

class EPanCamera : public Event
{
public:
    EPanCamera( double dToFire );
	void fire() const;
};