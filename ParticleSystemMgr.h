#pragma once
#include <list>
#include "Singleton.h"
#include "Object.h"
using std::list;
using tlib::Vector3f;
using tlib::Object;

// Forward declarations
class ParticleSystem;
class PSLaser;
class PSScorchMarks;
class PSSmallExplosion;
class PSSmoke;

typedef list<ParticleSystem*> PSList;

class ParticleSystemMgr : public Singleton<ParticleSystemMgr>
{
    friend Singleton<ParticleSystemMgr>;

private:
    // The list of particle systems
    PSList m_vPSList;

public:
    /**
     * 
     */
    PSLaser* addLaser( Object *oOwner,
                       float vfCorrect[], 
                       float vEmitterOffset,
                       float vfLaserColor[] );

    /**
     * 
     */
    PSSmallExplosion* addSmallExplosion( const Vector3f &vSysPos );

    /**
     * 
     */
    PSSmoke* addSmoke( const Vector3f &vSysPos );

    /**
     * 
     */
    PSScorchMarks* addScorchMark();

    /**
     * 
     */
    void update();

    /**
     * 
     */
    void render();

    /**
     * 
     */
    void remove( ParticleSystem *ps );

private:
    /**
     * Constructor
     */
    ParticleSystemMgr();


    /**
     * Destructor
     */
    ~ParticleSystemMgr();

    /**
     * 
     */
    ParticleSystem* checkForSameType( int iType );

}; // end of ParticleSystemMgr class

typedef ParticleSystemMgr PSManager;