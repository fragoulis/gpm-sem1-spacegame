#include <windows.h>
#include <gl/gl.h>
#include "DoorRenderer.h"
#include "Door.h"
#include "VisualBox.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

vector<Door*> DoorRenderer::m_vDoors;

void DoorRenderer::init()
{
    _LOG("Initializing door renderer...");
    Config cfg("config.txt");
    cfg.loadBlock("door_panel");

    // Get panel's half dimensions
    float fvDim[3];
    cfg.getFloat("half_dim", fvDim, 3);

    // Initialize visual component
    // We will use this single component to draw all door panels
    // hence all the doors
    setComponent( new OCVisualBox( Vector3f( fvDim ) ) );
}

void DoorRenderer::render()
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

            // Draw the door's panels
            const DoorPanel& leftPanel = (*iter)->getPanel(DoorPanel::Left);
            m_vPos.xyz( leftPanel.getPos() );
            m_qDir.wxyz( leftPanel.getDir() );
            cBox->render();

            const DoorPanel& rightPanel = (*iter)->getPanel(DoorPanel::Right);
            m_vPos.xyz( rightPanel.getPos() );
            m_qDir.wxyz( rightPanel.getDir() );
            cBox->render();

            const DoorPanel& topPanel = (*iter)->getPanel(DoorPanel::Top);
            m_vPos.xyz( topPanel.getPos() );
            m_qDir.wxyz( topPanel.getDir() );
            cBox->render();

            const DoorPanel& btmPanel = (*iter)->getPanel(DoorPanel::Bottom);
            m_vPos.xyz( btmPanel.getPos() );
            m_qDir.wxyz( btmPanel.getDir() );
            cBox->render();
        }
        glPopMatrix();

    } // end for( )

} // end render()