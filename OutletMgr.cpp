#include <windows.h>
#include <gl/gl.h>
#include "OutletMgr.h"
#include "Outlet.h"
#include "VisualBox.h"
#include "Material.h"
#include "Animation.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Outlet*> OutletMgr::m_vOutlets;

void OutletMgr::init()
{
    _LOG("Initializing outlet renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("outlet");

    // Get the outlet's bounding box which incidentaly is the same
    // as its [half] dimensions
    float fvDim[3];
    cfg.getFloat("bbox", fvDim, 3);
    
    // Initialize visual component
    // We will use this single component to draw all power outlets
    setComponent( new OCVisualBox( Vector3f( fvDim ) ) );
}

// ----------------------------------------------------------------------------
void OutletMgr::render()
{
    // Get visual component which will draw all power outlets
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");
    IOCMaterial *cMat;

    vector<Outlet*>::const_iterator iter;
    for( iter = m_vOutlets.begin();
         iter != m_vOutlets.end();
         iter++ )
    {
        // Apply the material for the outlet
        cMat = (IOCMaterial*)(*iter)->getComponent("material");
        cMat->apply();

        // Draw the door's panels
        m_vPos.xyz( (*iter)->getPos() );
        m_qDir.wxyz( (*iter)->getDir() );
        cBox->render();

    } // end for( )

} // end render()

// ----------------------------------------------------------------------------
void OutletMgr::update()
{
    IOCAnimation* cAnim;

    vector<Outlet*>::const_iterator iter;
    for( iter = m_vOutlets.begin();
         iter != m_vOutlets.end();
         iter++ )
    {
        cAnim = (IOCAnimation*)(*iter)->getComponent("animation");
        cAnim->update();
    }
} // end update()