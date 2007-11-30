#include <windows.h>
#include <gl/gl.h>
#include "ForcefieldMgr.h"
#include "Forcefield.h"
#include "VisualBox.h"
#include "SimpleMaterial.h"
#include "Animation.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

ForcefieldList ForcefieldMgr::m_vForcefields;

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

// ----------------------------------------------------------------------------
void ForcefieldMgr::update()
{
    IOCAnimation* cAnim;

    Forcefield *obj;
    ForcefieldList toKill;
    ForcefieldList::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        obj = *iter;

        if( ObjectMgr::Instance().isCulled( obj ) ) {
            // If object is not active dont bother updating it
            // since it is culled
            continue;
        }

        cAnim = (IOCAnimation*)obj->getComponent("animation");
        // If object has finished its animation kill it
        if( cAnim->isDone() )
        {
            toKill.push_back( obj );
            continue;
        }

        cAnim->update();
    }

    for( iter = toKill.begin(); 
         iter != toKill.end(); 
         ++iter )
    {
         remove( *iter );
    }

} // end update()

// ----------------------------------------------------------------------------
void ForcefieldMgr::render()
{
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    // Get visual component which will draw all door panels
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");

    Forcefield *obj;
    ForcefieldList::const_iterator iter;
    for( iter = m_vForcefields.begin();
         iter != m_vForcefields.end();
         iter++ )
    {
        obj = *iter;

        // If it's not active don't render as it is culled
        if( !obj->isActive() ) continue;

        // Apply the material for the outlet
        IOCMaterial *cMat = (IOCMaterial*)obj->getComponent("material");
        cMat->apply();

        // Draw the door's panels
        m_vPos.xyz( obj->getPos() );
        m_qDir.wxyz( obj->getDir() );
        cBox->render();

    } // end for( )

    glDisable(GL_BLEND);

} // end render()

// ----------------------------------------------------------------------------
void ForcefieldMgr::remove( Forcefield *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vForcefields.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}