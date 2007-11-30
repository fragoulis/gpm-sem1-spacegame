#include "Reactor.h"
#include "Logger.h"
#include "Config.h"
#include "Texture.h"
#include "SimpleMaterial.h"
#include "SingleTexture.h"
#include "VisualVertexArraySphere.h"
#include "CollisionBSphere.h"
#include "Movement.h"

using namespace tlib;

Reactor::Reactor() {}

void Reactor::init()
{
    _LOG("Setting up reactor...");

    Config cfg("config.txt");
    cfg.loadBlock("reactor");

    // Initialize position
    float vfPos[3];
    cfg.getFloat("init_pos", vfPos, 3);
    m_vPos.xyz( vfPos );

    // Initialize member variables
    cfg.getFloat("rot_factor", &m_fRotFactor );

    // Read part's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read part's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read part's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Read reactor's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Initialize material component
    setComponent( new OCSimpleMaterial( 
        Color::black(),
        Color(vfColor),
        Color::null()) 
        );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iStacks, iSlices ) );

    // Get panel's texture
    string sTexture;
    cfg.getString("texture", sTexture );

    // Initialize texture component
    setComponent( new OCSingleTexture( sTexture.c_str() ) );

    // Initialize collision component
    setComponent( new OCCollisionBSphere( fRadius ) );

    // Initialize collision response component
    //setComponent( new ReactorCollisionResponse );

} // end setup()

void Reactor::render()
{ // Render reactor
    IOCVisual *cVisual = (IOCVisual*)getComponent("visual");
    _ASSERT(cVisual!=0); 
 
    // Modify texture matrix to simulate a texture rotation
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    static float fAngle = 0.0f;

    // Rotate matrix
    glRotatef( fAngle, 0.0f, 0.0f, 1.0f );

    // Increase step
    fAngle += m_fRotFactor * IOCMovement::DeltaTime();

    // Reload modelview matrix
    glMatrixMode(GL_MODELVIEW);

    // Render the reactor

    glPushMatrix();
        /*float matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        matrix[0] = matrix[5] = matrix[10] = matrix[11] = 1.0f;
        matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0f;
        matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0f;
        glLoadMatrixf(matrix);*/

        //glScalef(5.0f,5.0f,5.0f);
        cVisual->render();
    glPopMatrix();
    
    // Reset texture matrix
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}