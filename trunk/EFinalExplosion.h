#pragma once
#include "Event.h"
#include "Vector3.h"
using tlib::Vector3f;

class EFinalExplosion : public Event
{
private:
	//const Vector3f &m_vPos;
    Vector3f m_vPos;
public:
	EFinalExplosion( const Vector3f &vPos, double dToFire );
	void fire() const;
};