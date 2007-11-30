#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "PSScorchMarks.h"
#include "PSSmallExplosion.h"
#include "PSSmoke.h"

ParticleSystemMgr::ParticleSystemMgr()
{}

ParticleSystemMgr::~ParticleSystemMgr()
{
    // Clear particle system list
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); 
         iter != m_vPSList.end(); 
         ++iter )
    {
        delete *iter;
        *iter = 0;
    }
}

// ----------------------------------------------------------------------------
ParticleSystem* ParticleSystemMgr::checkForSameType( int iType )
{
    // Search for an unused/expired system
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); 
         iter != m_vPSList.end(); 
         ++iter )
    {
        if( (*iter)->isExpired() )
            if( (*iter)->isType( iType ) )
                return *iter;
    }

    return 0;
}

// ----------------------------------------------------------------------------
PSLaser* ParticleSystemMgr::addLaser( Object *oOwner,
                                      float vfCorrect[], 
                                      float vEmitterOffset,
                                      float vfLaserColor[] )
{
    // Create the new laser particle system
    PSLaser *ps = new PSLaser;
    ps->init( oOwner, vfCorrect, vEmitterOffset, vfLaserColor );

    // Push it pack to the list
    m_vPSList.push_back( (ParticleSystem*)ps );

    return ps;
}

// ----------------------------------------------------------------------------
PSSmallExplosion* ParticleSystemMgr::addSmallExplosion( const Vector3f &vSysPos )
{
    PSSmallExplosion *ps;

    // Before allcating a new system
    // Search for an unused/expired system
    if( !(ps = (PSSmallExplosion*)checkForSameType( ParticleSystem::SmallExplosion )) )
    {
        // Create the explosion particle system
        ps = new PSSmallExplosion;
        ps->init( vSysPos );

        // Push it pack to the list
        m_vPSList.push_back( (ParticleSystem*)ps );
    } else {
        ps->setPos( vSysPos );
    }
    
    return ps;
}

// ----------------------------------------------------------------------------
PSSmoke* ParticleSystemMgr::addSmoke( const Vector3f &vSysPos )
{
    PSSmoke *ps;

    // Before allcating a new system
    // Search for an unused/expired system
    if( !(ps = (PSSmoke*)checkForSameType( ParticleSystem::Smoke )) )
    {
        // Create the explosion particle system
        ps = new PSSmoke;
        ps->init( vSysPos );

        // Push it pack to the list
        m_vPSList.push_back( (ParticleSystem*)ps );
    } else {
        ps->setPos( vSysPos );
    }
    
    return ps;
}

// ----------------------------------------------------------------------------
PSScorchMarks* ParticleSystemMgr::addScorchMark()
{
    // Create the scortch mark particle system
    PSScorchMarks *ps = new PSScorchMarks;
    ps->init();

    // Push it pack to the list
    m_vPSList.push_back( (ParticleSystem*)ps );

    return ps;
}

// ----------------------------------------------------------------------------
void ParticleSystemMgr::remove( ParticleSystem *value )
{
    _ASSERT(value!=0);

    // Remove it from the particle system list
    m_vPSList.remove( value );

    // Delete particle system from memory
    delete value;
    value = 0;
}

// ----------------------------------------------------------------------------
void ParticleSystemMgr::update()
{
    PSList toKill;
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); iter != m_vPSList.end(); ++iter )
    {
        _ASSERT((*iter)!=0);
        ParticleSystem *ps = *iter;

        // If a particle system has expired
        if( ps->hasExpired() ) 
        {
            if( !ps->getEmitter().getPAlive().size() ) {
                //toKill.push_back( ps );
                continue;
            } else {
                // Turn of its emitter
                ps->getEmitter().stop();
            }
        }

        ps->update();
    }

    //for( iter = toKill.begin(); iter != toKill.end(); ++iter ) {
    //     remove( *iter );
    //}

} // end update()

// ----------------------------------------------------------------------------
void ParticleSystemMgr::render()
{
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); iter != m_vPSList.end(); ++iter )
    {
        _ASSERT((*iter)!=0);
        (*iter)->render();
    }
}