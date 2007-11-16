#include <windows.h>
#include <gl/gl.h>
#include "TurretMgr.h"
#include "Turret.h"
//#include "Animation.h"
#include "Visual.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Turret*> TurretMgr::m_vTurrets;

void TurretMgr::render()
{
    // Draw all turrets
    vector<Turret*>::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         iter++ )
    {
         // Get visual component
        IOCVisual *cModel = (IOCVisual*)(*iter)->getComponent("visual");
        cModel->render();
    }

} // end render()

void TurretMgr::update()
{
    /*IOCAnimation* cAnim;

    vector<Turret*>::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        cAnim = (IOCAnimation*)(*iter)->getComponent("animation");
        cAnim->update();
    }*/
} // end update()

void TurretMgr::destroy()
{
    // 
    vector<Turret*>::const_iterator iter;
    for( iter = m_vTurrets.begin();
         iter != m_vTurrets.end();
         iter++ )
    {
        (*iter)->clearComponentList();
    }
}