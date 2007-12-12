#include <windows.h>
#include <gl/gl.h>
#include "OutletMgr.h"
#include "Outlet.h"
#include "VisualBox.h"
#include "Material.h"
#include "Animation.h"
#include "ObjectMgr.h"
#include "SingleTexture.h"
#include "Shader.h"
#include "Tile3d.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

OutletList OutletMgr::m_vOutlets;

void OutletMgr::init()
{
    // Set object's type
    setType( OUTLET );

    _LOG("Initializing outlet renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("outlet");

    string sTexture;
    cfg.getString("texture", sTexture);
    setComponent( new OCSingleTexture( sTexture.c_str() ) );

    // Get the outlet's bounding box which incidentaly is the same
    // as its [half] dimensions
    float vfDim[3];
    cfg.getFloat("bbox", vfDim, 3);
    
    // Initialize visual component
    // We will use this single component to draw all power outlets
    OCVisualBox *cVBox = new OCVisualBox;
    setComponent( cVBox );
    cVBox->init( Vector3f( vfDim ) );

    // Initialize shader object
    setComponent( new OCShader( ShaderMgr::POINT_AND_SPOT_LIGHT_SINGLE_TEX ) );
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
        getPos().xyz( obj->getPos() );
        getDir().wxyz( obj->getDir() );
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

// ----------------------------------------------------------------------------
Outlet* OutletMgr::add( Tile3d *oTile )
{
    // Allocate object
    Outlet *obj = new Outlet;

    // Save it as this tile's occupant
    oTile->setOccupant( (Object*)obj );

    // Push it to the list
    m_vOutlets.push_back( obj );

    return obj;
}