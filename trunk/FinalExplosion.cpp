#include "FinalExplosion.h"
#include "ParticleSystemMgr.h"
#include "PSCommon.h"

FinalExplosion::FinalExplosion( const Vector3f &vPos, double dToFire ):
Event(dToFire),
m_vPos(vPos)
{}

void FinalExplosion::fire() const
{
	// Create a big explosion of particles
	PSCommon *ps = PSManager::Instance().addSystem(
		PSManager::BIG_EXPLOSION, 
		m_vPos
		);

	ps->start();
}