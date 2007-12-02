#include "MyWindow.h"
// System
#include <iostream>
#include <cstdlib>
// Misc
#include "Config.h"
#include "Math.h"
// Objects
#include "TPCamera.h"
#include "PSSmallExplosion.h"
#include "PSSmoke.h"
// Components
#include "Movement.h"
#include "SpaceshipKeyboard.h"
// Singletons
#include "CameraMgr.h"
#include "ObjectMgr.h"
#include "ParticleSystemMgr.h"
#include "Tilemap.h"
#include "Logger.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"

using namespace std;
using namespace tlib;

GLuint program;
GLuint tplane, plane;
GLuint tex1, tex2;

MyWindow::MyWindow() 
{   
    SetSize( 1024, 768 );
    SetPosition(100,100);
}

void MyWindow::OnCreate() 
{
    GLWindowEx::OnCreate();
    SetFont( 30, "Times" );
    //SetFullscreen();
    SetCursor( CRNone );

    glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
    //// enable backface culling
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    //// ----SETUP SOME DEFAULT GLOBAL LIGHTING -------
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    float _light[][4] = {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 15.0f, 15.0f, 690.0f, 1.0f }
    };
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    //
    //// Low ambient
    Config cfg("config.txt");
    float gAmbient[4];
    cfg.loadBlock("global");
    cfg.getFloat("light", gAmbient, 4);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, gAmbient);

    //// Open log file
    Logger::Instance().open( Logger::ERROR_LOG, true );

    ObjectMgr::Instance().init();
    m_DoorMgr.init();
    m_ForcefieldMgr.init();
    m_OutletMgr.init();
    m_TurretMgr.init( (Object*)&ObjectMgr::Instance().getShip() );

    // Third person
    TPCamera *camStalker = CameraMgr::Instance().add<TPCamera>("stalker");
    camStalker->init( (Object*)&ObjectMgr::Instance().getShip() );
    CameraMgr::Instance().activate("stalker");

    // First person
    //FPCamera *camStalker = CameraMgr::Instance().add<TPCamera>("stalker");
    //cCamOri = (IOCOrientation*)camStalker->getComponent("orientation");
    //camStalker->setPos( Vector3f(-40,40,0) );
    //cCamOri->lookAt( m_Ship.getPos() );
}

void MyWindow::OnDisplay() 
{
    // save time passed between frames
    IOCMovement::DeltaTime( (float)App::GetDeltaTime() );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ----------------------------------------------
    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(Width()) / float(Height());
    gluPerspective( 80, ratio, 0.9f, 1000.0f );
    // reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,Width(),Height());
    glEnable( GL_LIGHTING );

    CameraMgr::Instance().apply();

    // Render spacestation
    m_DoorMgr.render();
    m_BladeMgr.render();
    m_OutletMgr.render();
    m_TurretMgr.render();
    ObjectMgr::Instance().render();
    // Draw forcefields at the end because of the transparency
    // What if we do it in shaders???? 
    PSManager::Instance().render();
    m_ForcefieldMgr.render();

    drawInterface();

    SwapBuffers();
}

GLuint MyWindow::compilePlane(bool texture)
{
    GLuint id = glGenLists(1);
    glNewList(id, GL_COMPILE);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            //glTexCoord2f(0,0);
            if(texture){
                glMultiTexCoord2f( GL_TEXTURE0_ARB, 0.0f, 0.0f );
                glMultiTexCoord2f( GL_TEXTURE1_ARB, 0.0f, 0.0f );
            }
            glVertex2f(0,0);
            //glTexCoord2f(1,0);
            if(texture){
                glMultiTexCoord2f( GL_TEXTURE0_ARB, 1.0f, 0.0f );
                glMultiTexCoord2f( GL_TEXTURE1_ARB, 1.0f, 0.0f );
            }
            glVertex2f(10,0);
            //glTexCoord2f(1,1);
            if(texture){
                glMultiTexCoord2f( GL_TEXTURE0_ARB, 1.0f, 1.0f );
                glMultiTexCoord2f( GL_TEXTURE1_ARB, 1.0f, 1.0f );
            }
            glVertex2f(10,10);
            //glTexCoord2f(0,1);
            if(texture){
                glMultiTexCoord2f( GL_TEXTURE0_ARB, 0.0f, 1.0f );
                glMultiTexCoord2f( GL_TEXTURE1_ARB, 0.0f, 1.0f );
            }
            glVertex2f(0,10);
        glEnd();
    glEndList();

    return id;
}

void MyWindow::OnIdle() 
{
	// Update spaceship and its shield
    ObjectMgr::Instance().update();

    //// Update third-person camera
    TPCamera *camTP = (TPCamera*)CameraMgr::Instance().get("stalker");
    camTP->update();

    //// Update barriers, defence guns and outlets
    m_DoorMgr.update();
    m_BladeMgr.update();
    m_ForcefieldMgr.update();
    m_TurretMgr.update();
    m_OutletMgr.update();

    // --------------------------------------------------------------------
    ObjectMgr::Instance().checkCollision();
    // At last update particles and check collisions with the tiles and
    // with the object's within them
    PSManager::Instance().update();

    Redraw();
}

void MyWindow::OnResize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(w) / float(h);
    gluPerspective( 60, ratio, 0.9f, 1000.0f );
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);
}

void MyWindow::OnDestroy() 
{
    // Special case
    // We must destroy GXModel here to avoid memory leaks
    // So, to avoid destroying the gxmodel component of the 
    // spaceship anywhere else, we clear the component list here
    // explicitly.
    m_TurretMgr.clearComponentList();
    
    // Explicitly destroy singletons
    ObjectMgr::Destroy();
    TextureMgr::Destroy();
    PSManager::Destroy();
    Tilemap::Destroy();
    CameraMgr::Destroy();
    Logger::Destroy();
    ShaderMgr::Destroy();

    //glDeleteLists(tplane,1);
    //glDeleteLists(plane,1);
}

void MyWindow::OnKeyboard(int key, bool down) 
{
    // Here we'll add, GOD MODE, CAMERA TOGGLE, RECORD MODE
	key = tolower(key);
    if( key == 'q' || key == 27 ) Close();

    if( key == 'l' ) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else if( key == 'k' ) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // Capture keyboard for spaceship
    OCKeyboard *cShipCont = 
        (OCKeyboard*)ObjectMgr::Instance().getShip().getComponent("controller");
    cShipCont->capture( key, down );

    Redraw();
}	

void MyWindow::drawInterface()
{
    // ----------------------------------------------
    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( 0, Width(), 0, Height() );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable( GL_LIGHTING );

    // Draw health status
    glColor3f( 1.0f, 1.0f, 0.0f );
    glRasterPos2i( 10, 10 );
    Printf( "Health: %i", ObjectMgr::Instance().getShip().getHealth() );
}