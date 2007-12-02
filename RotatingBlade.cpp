#include <windows.h>
#include <gl/gl.h>
#include "RotatingBlade.h"
#include "LinearMovement.h"
#include "BladeAnimation.h"
#include "CollisionBBox.h"
#include "Visual.h"
#include "Config.h"
#include "SingleTexture.h"
using namespace tlib;

float RotatingBlade::DistToDie;

// ----------------------------------------------------------------------------
RotatingBlade::RotatingBlade()
{
    // Read the bouding box for the door and 
    // pass it to the parent constructor
    Config cfg("config.txt");
    cfg.loadBlock("blade_system");

    // Read bounding box for the door
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);
    DistToDie = vfBBox[1];

    // Read the system's speed for when shutting off
    float fVelocity;
    cfg.getFloat("velocity", &fVelocity);

    // Set the barrier's bounding box
    // We are dividing by two because all of our bounding 
    // boxes are expressed in half sized dimensions
    OCCollisionBBox *cColBox = (OCCollisionBBox*)getComponent("collision");
    cColBox->setBBox( Vector3f( vfBBox ) * 0.5f );

    // Initialize movement component
    setComponent( new OCLinearMovement( fVelocity, Vector3f::Up() ) );

    // Initialize animation component
    setComponent( new BladeAnimation );

    // 
    //setComponent( new OCSingleTexture( "textures/beam.jpg" ) );

    // Initialize system parts
    m_oBlade.init();
    m_oPoll.init();
    m_oBase.init();
}

// ----------------------------------------------------------------------------
void RotatingBlade::render()
{
    glPushMatrix();
    {
        // Position system
        glTranslatef( m_vPos.x(), m_vPos.y(), m_vPos.z() );
        // Rotate system
        float vfRotMatrix[16];
        m_qDir.toMatrix(vfRotMatrix);
        glMultMatrixf(vfRotMatrix);

        // Draw system poll
        IOCVisual *cVis = (IOCVisual*)m_oPoll.getComponent("visual");
        _ASSERT(cVis!=0); cVis->render();

        // Draw system base
        cVis = (IOCVisual*)m_oBase.getComponent("visual");
        _ASSERT(cVis!=0);
        cVis->render();

        // Draw system blade
        cVis = (IOCVisual*)m_oBlade.getComponent("visual");
        _ASSERT(cVis!=0); cVis->render();
    }
    glPopMatrix();
}