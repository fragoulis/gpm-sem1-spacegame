#include <windows.h>
#include <gl/gl.h>
#include "ForcefieldMgr.h"
#include "Forcefield.h"
#include "VisualBox.h"
#include "SimpleMaterial.h"
#include "Animation.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Forcefield*> ForcefieldMgr::m_vForcefields;

void ForcefieldMgr::init()
{
    _LOG("Initializing forcefield renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("forcefield");

    // Read the bounding box of the forcefield
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);
    
    // Initialize visual component
    // We will use this single component to draw all forcefields
    setComponent( new OCVisualBox( Vector3f( vfBBox ) * 0.5f ) );
}

void ForcefieldMgr::render()
{
    // Get visual component which will draw all door panels
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");

    vector<Forcefield*>::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        // Apply the material for the outlet
        IOCMaterial *cMat = (IOCMaterial*)(*iter)->getComponent("material");
        cMat->apply();

        // Draw the door's panels
        m_vPos.xyz( (*iter)->getPos() );
        m_qDir.wxyz( (*iter)->getDir() );
        cBox->render();

    } // end for( )

} // end render()

void ForcefieldMgr::update()
{
    IOCAnimation* cAnim;

    vector<Forcefield*>::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        cAnim = (IOCAnimation*)(*iter)->getComponent("animation");
        cAnim->update();
    }
} // end update()