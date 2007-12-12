#pragma once
#include "Event.h"
#include "Vector3.h"
using tlib::Vector3f;

class FinalExplosion : public Event
{
private:
	const Vector3f &m_vPos;
public:
	FinalExplosion( const Vector3f &vPos, double dToFire );
	void fire() const;
};