#include <windows.h>
#include <gl/gl.h>
#include "OutletMgr.h"
#include "Outlet.h"
#include "VisualBox.h"
#include "Material.h"
#include "Animation.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

OutletList OutletMgr::m_vOutlets;

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

    Outlet *obj;
    OutletList::const_iterator iter;
    for( iter = m_vOutlets.begin();
         iter != m_vOutlets.end();
         iter++ )
    {
        obj = *iter;

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            continue;
        }

        // Apply the material for the outlet
        cMat = (IOCMaterial*)obj->getComponent("material");
        cMat->apply();

        // Draw the door's panels
        m_vPos.xyz( obj->getPos() );
        m_qDir.wxyz( obj->getDir() );
        cBox->render();

    } // end for( )

} // end render()

// ----------------------------------------------------------------------------
void OutletMgr::update()
{
    IOCAnimation* cAnim;

    Outlet *obj;
    OutletList::const_iterator iter;
    for( iter = m_vOutlets.begin();
         iter != m_vOutlets.end();
         iter++ )
    {
        obj = *iter;

        // If it's not active don't render as it is culled
        if( !obj->isActive() ) continue;

        cAnim = (IOCAnimation*)obj->getComponent("animation");
        cAnim->update();
    }
} // end update()