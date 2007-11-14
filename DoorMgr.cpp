#include <windows.h>
#include <gl/gl.h>
#include "DoorMgr.h"
#include "Door.h"
#include "VisualBox.h"
#include "SimpleMaterial.h"
#include "Animation.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Door*> DoorMgr::m_vDoors;

void DoorMgr::init()
{
    _LOG("Initializing door renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("door_panel");

    // Get panel's half dimensions
    float fvDim[3];
    cfg.getFloat("half_dim", fvDim, 3);

    // Initialize material component
    //setComponent( new OCSimpleMaterial(Color::black(),Color::red(),Color::null()) );
    setComponent( new OCSimpleMaterial );
    
    // Initialize visual component
    // We will use this single component to draw all door panels
    // hence all the doors
    setComponent( new OCVisualBox( Vector3f( fvDim ) ) );
}

void DoorMgr::render()
{
    // Get visual component which will draw all door panels
    IOCVisual *cBox = (IOCVisual*)getComponent("visual");

    vector<Door*>::const_iterator iter;
    for( iter = m_vDoors.begin();
         iter != m_vDoors.end();
         iter++ )
    {
        // For door's position shorthand
        const Vector3f& vDoorPos = (*iter)->getPos();
        glPushMatrix();
        {
            // Position door
            glTranslatef( vDoorPos.x(), vDoorPos.y(), vDoorPos.z() );
            // ... and rotate door
            float vfRotMatrix[16];
            (*iter)->getDir().toMatrix(vfRotMatrix);
            glMultMatrixf(vfRotMatrix);

            // Draw the door's panels
            m_vPos.xyz( (*iter)->leftPanel()->getPos() );
            m_qDir.wxyz( (*iter)->leftPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( (*iter)->rightPanel()->getPos() );
            m_qDir.wxyz( (*iter)->rightPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( (*iter)->topPanel()->getPos() );
            m_qDir.wxyz( (*iter)->topPanel()->getDir() );
            cBox->render();

            m_vPos.xyz( (*iter)->bottomPanel()->getPos() );
            m_qDir.wxyz( (*iter)->bottomPanel()->getDir() );
            cBox->render();
        }
        glPopMatrix();

    } // end for( )

} // end render()

void DoorMgr::update()
{
    IOCAnimation* cAnim;

    vector<Door*>::const_iterator iter;
    for( iter = m_vDoors.begin();
         iter != m_vDoors.end();
         iter++ )
    {
        cAnim = (IOCAnimation*)(*iter)->getComponent("animation");
        cAnim->update();
    }
} // end update()