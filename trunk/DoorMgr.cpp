#include <windows.h>
#include <gl/gl.h>
#include "DoorMgr.h"
#include "Door.h"
#include "VisualBox.h"
#include "SimpleMaterial.h"
#include "SingleTexture.h"
#include "Animation.h"
#include "Shader.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

DoorList DoorMgr::m_vDoors;

void DoorMgr::init()
{
    _LOG("Initializing door renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("door_panel");

    // Get panel's half dimensions
    float fvDim[3];
    cfg.getFloat("half_dim", fvDim, 3);

    // Get panel's texture
    string sTexture;
    cfg.getString("texture", sTexture );

    // Initialize material component
    //setComponent( new OCSimpleMaterial(Color::black(),Color::red(),Color::null()) );
    setComponent( new OCSimpleMaterial );
    
    // Initialize texture component
    setComponent( new OCSingleTexture( sTexture.c_str() ) );

    // Initialize visual component
    // We will use this single component to draw all door panels
    // hence all the doors
    OCVisualBox *cVBox = new OCVisualBox;
    setComponent( cVBox );
    cVBox->init( Vector3f( fvDim ) );
}

// ----------------------------------------------------------------------------
void DoorMgr::update()
{
    IOCAnimation* cAnim;

    Door *obj;
    DoorList toKill;
    DoorList::const_iterator iter;
    for( iter = m_vDoors.begin();
         iter != m_vDoors.end();
         ++iter )
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
void DoorMgr::render()
{
    // Get visual component which will draw all door panels
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");
    
    Door *obj;
    DoorList::const_iterator iter;
    for( iter = m_vDoors.begin();
         iter != m_vDoors.end();
         ++iter )
    {
        obj = *iter;

        // If it's not active don't render as it is culled
        if( !obj->isActive() ) continue;

        // For door's position shorthand
        const Vector3f& vDoorPos = obj->getPos();
        glPushMatrix();
        {
            // Position door
            glTranslatef( vDoorPos.x(), vDoorPos.y(), vDoorPos.z() );
            // ... and rotate door
            float vfRotMatrix[16];
            obj->getDir().toMatrix(vfRotMatrix);
            glMultMatrixf(vfRotMatrix);

            // Draw the door's panels
            m_vPos.xyz( obj->leftPanel()->getPos() );
            m_qDir.wxyz( obj->leftPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( obj->rightPanel()->getPos() );
            m_qDir.wxyz( obj->rightPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( obj->topPanel()->getPos() );
            m_qDir.wxyz( obj->topPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( obj->bottomPanel()->getPos() );
            m_qDir.wxyz( obj->bottomPanel()->getDir() );
            cBox->render();
        }
        glPopMatrix();

    } // end for( )

} // end render()

// ----------------------------------------------------------------------------
void DoorMgr::remove( Door *value )
{
    _ASSERT(value!=0);

    // Remove it from the object list
    m_vDoors.remove( value );

    // Delete object system from memory
    delete value;
    value = 0;
}