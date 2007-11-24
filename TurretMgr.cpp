#include "TurretMgr.h"
#include "Turret.h"
#include "TurretAI.h"
#include "VisualGXModel.h"
#include "QuatRotation.h"
//#include "Visual.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Turret*> TurretMgr::m_vTurrets;

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

void TurretMgr::render()
{
    IOCVisual *cModel = (IOCVisual*)getComponent("visual");

    // Draw all turrets
    vector<Turret*>::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         iter++ )
    {
        // If object has finished its animation, and its not visible
        // skip it
        if( !(*iter)->isActive() ) continue;

         // Get visual component
        m_vPos.xyz( (*iter)->getPos() );
        m_qDir.wxyz( (*iter)->getDir() );
        cModel->render();
    }

} // end render()

void TurretMgr::update()
{
    IOCAI* cAI = (IOCAI*)getComponent("ai");
    OCQuatRotation *cShipOri;

    vector<Turret*>::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         iter++ )
    {
        if( !(*iter)->isActive() ) continue;

        // Get turret orientation by updating the AI component
        //m_vPos.xyz( (*iter)->getPos() );
        //m_qDir.wxyz( (*iter)->getDir() );
        cAI->update( *iter );

        // Update each turrets individual orientation
        cShipOri = (OCQuatRotation*)(*iter)->getComponent("orientation");
        cShipOri->update();
    }
} // end update()