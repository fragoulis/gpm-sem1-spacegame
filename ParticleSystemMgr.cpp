#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "PSScorchMarks.h"
#include "PSCommon.h"
#include "PSTemplate.h"
#include "Timer.h"
#include "Logger.h"
using tlib::Logger;

ParticleSystemMgr::ParticleSystemMgr()
{
    memset( m_Templates, 0, sizeof(m_Templates) );
    m_Templates[EXPLOSION]      = new PSTemplate("explosion");
    m_Templates[BIG_EXPLOSION]  = new PSTemplate("big_explosion");
    m_Templates[SMOKE]          = new PSTemplate("smoke");
}

ParticleSystemMgr::~ParticleSystemMgr()
{
    _LOG("Deleting particle system manager");
    // Clear particle system list
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); 
         iter != m_vPSList.end(); 
         ++iter )
    {
        delete *iter;
        *iter = 0;
    }

    for( int i=0; i<NUM_OF_TEMPLATES; ++i ) {
        if( m_Templates ) {
            delete m_Templates[i];
            m_Templates[i] = 0;
        }
    }

}

// ----------------------------------------------------------------------------
PSLaser* ParticleSystemMgr::addLaser( Object *oOwner,
                                      float vEmitterOffset,
                                      float vfLaserColor[] )
{
    // Create the new laser particle system
    PSLaser *ps = new PSLaser;
    ps->init( oOwner, vEmitterOffset, vfLaserColor );

    // Push it pack to the list
    m_vPSList.push_back( (ParticleSystem*)ps );

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
PSCommon* ParticleSystemMgr::addSystem( PSType iType, const Vector3f &vSysPos )
{
    PSCommon *ps = new PSCommon;
    ps->init( *(m_Templates[iType]), vSysPos );

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
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); iter != m_vPSList.end(); )
    {
        _ASSERT((*iter)!=0);
        ParticleSystem *ps = *iter;

        // If a particle system has expired
        if( !ps->getTimer()->isRunning() ) 
        {
            if( ps->getEmitter().getPAlive().size() ) {
                // Turn of its emitter
                ps->getEmitter().stop();
            } 
            else {
                iter = m_vPSList.erase(iter);
                delete ps;
                ps = 0;
                continue;
            }
        }

        ps->update();

        ++iter;
    }

} // end update()

// ----------------------------------------------------------------------------
void ParticleSystemMgr::render()
{
    PSList::iterator iter;
    for( iter = m_vPSList.begin(); iter != m_vPSList.end(); ++iter )
    {
        _ASSERT((*iter)!=0);
        ParticleSystem *ps = *iter;
        if( ps->getEmitter().getPAlive().size() ) {
            ps->render();
        }
    }
}