#include "TurretMgr.h"
#include "Turret.h"
#include "TurretAI.h"
#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "VisualGXModel.h"
#include "QuatRotation.h"
#include "Vitals.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

TurretList TurretMgr::m_vTurrets;

// ----------------------------------------------------------------------------
void TurretMgr::init( Object *obj )
{
    Config cfg("config.txt");
    cfg.loadBlock("turret");

    // Initialize visual component
    string sModel;
    float fScale;
    cfg.getString("model", sModel );
    cfg.getFloat("scale", &fScale );
    setComponent( new OCGXModel( sModel.c_str(), fScale ) );

    // Initialize AI component
    setComponent( new TurretAI( obj ) );
}

// ----------------------------------------------------------------------------
void TurretMgr::update()
{
    IOCAI *cAI = (IOCAI*)getComponent("ai");
    OCQuatRotation *cShipOri;

    IOCVitals *cVitals;
    TurretList toKill;
    TurretList::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         ++iter )
    {
        _ASSERT((*iter)!=0);
        // Cache dereference
        Turret *obj = *iter;

        if( !obj->isActive() ) 
        {
            //toKill.push_back( obj );
            continue;
        }

        // Get turret orientation by updating the AI component
        // ??????????????
        cAI->update( obj );

        // Update each turrets individual orientation
        cShipOri = (OCQuatRotation*)obj->getComponent("orientation");
        cShipOri->update();

        // Update turret's health
        cVitals = (IOCVitals*)obj->getComponent("vitals");
        cVitals->update();

    } // end for( ... )

    for( iter = toKill.begin(); 
         iter != toKill.end(); 
         ++iter )
    {
         remove( *iter );
    }

} // end update()

// ----------------------------------------------------------------------------
void TurretMgr::render()
{
    IOCVisual *cModel = (IOCVisual*)getComponent("visual");

    // Draw all turrets
    TurretList::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         ++iter )
    {
        _ASSERT((*iter)!=0);

        if( !(*iter)->isActive() ) 
        {
            //toKill.push_back( obj );
            continue;
        }

         // Get visual component
        m_vPos.xyz( (*iter)->getPos() );
        m_qDir.wxyz( (*iter)->getDir() );
        cModel->render();
    }

} // end render()

// ----------------------------------------------------------------------------
void TurretMgr::remove( Turret *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vTurrets.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}