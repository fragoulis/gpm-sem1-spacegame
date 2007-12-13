#include "EFinalExplosion.h"
#include "ParticleSystemMgr.h"
#include "PSCommon.h"

EFinalExplosion::EFinalExplosion( const Vector3f &vPos, double dToFire ):
Event(dToFire),
m_vPos(vPos)
{}

void EFinalExplosion::fire() const
{
	// Create a big explosion of particles
	PSCommon *ps = PSManager::Instance().addSystem(
		PSManager::BIG_EXPLOSION, 
		m_vPos
		);

	ps->start();
}