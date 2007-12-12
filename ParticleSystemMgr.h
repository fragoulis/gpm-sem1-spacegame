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
class PSCommon;
class PSTemplate;

typedef list<ParticleSystem*> PSList;

class ParticleSystemMgr : public Singleton<ParticleSystemMgr>
{
    friend Singleton<ParticleSystemMgr>;

public:
    enum PSType {
        EXPLOSION,
        BIG_EXPLOSION,
        SMOKE,
        NUM_OF_TEMPLATES
    };

private:
    // The list of particle systems
    PSList m_vPSList;
    // The list of availabel templates
    PSTemplate *m_Templates[NUM_OF_TEMPLATES];

public:
    /**
     * 
     */
    PSLaser* addLaser( Object *oOwner,                 
                       float vEmitterOffset,
                       float vfLaserColor[] );

    /**
     * 
     */
    PSScorchMarks* addScorchMark();

    /**
     * 
     */
    PSCommon* addSystem( PSType iType, const Vector3f &vSysPos );

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

}; // end of ParticleSystemMgr class

typedef ParticleSystemMgr PSManager;