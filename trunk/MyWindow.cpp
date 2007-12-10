#include "MyWindow.h"
// System
#include <iostream>
#include <cstdlib>
// Misc
#include "Config.h"
#include "Math.h"
// Objects
#include "Logger.h"
#include "TPCamera.h"
#include "FPCamera.h"
//#include "PointLight.h"
#include "ClockTimeSource.h"
#include "FileTimeSource.h"
// Components
#include "SpaceshipKeyboard.h"
//#include "Visual.h"
// Singletons
#include "CameraMgr.h"
#include "ObjectMgr.h"
#include "ParticleSystemMgr.h"
#include "Tilemap.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"
#include "LightMgr.h"
#include "Clock.h"
#include "Recorder.h"

using namespace std;
using namespace tlib;
#include "PSBigExplosion.h"
PSBigExplosion *BigEx;

MyWindow::MyWindow() 
{   
    Config cfg("config.txt");
    cfg.loadBlock("display");
    
    int iDim[2];
    cfg.getInt("dimensions", iDim, 2);
    SetSize( iDim[0], iDim[1] );

    SetPosition(100,100);
}

// ----------------------------------------------------------------------------
void MyWindow::OnCreate() 
{
    GLWindowEx::OnCreate();
    _CLEAR_LOG
    // Start the application clock
    Clock::Instance().Start( new ClockTimeSource );

    Config cfg("config.txt");

    // Map application switches
    memset( AppControl, 0, sizeof(AppControl) );
    cfg.loadBlock("controls");
    cfg.getChar("fullscreen", &AppControlKey[FULLSCREEN] );
    cfg.getChar("camera",     &AppControlKey[CAMERA] );
    cfg.getChar("record",     &AppControlKey[RECORD] );
    cfg.getChar("collision",  &AppControlKey[COLLISION] );
    cfg.getChar("wireframe",  &AppControlKey[WIREFRAME] );
    cfg.getChar("god_mode",   &AppControlKey[GOD_MODE] );
    AppControlLabel[FULLSCREEN] = "Fullscreen";
    AppControlLabel[CAMERA]     = "Camera";
    AppControlLabel[RECORD]     = "Recording";
    AppControlLabel[COLLISION]  = "Collision";
    AppControlLabel[WIREFRAME]  = "Wireframe";
    AppControlLabel[GOD_MODE]   = "God Mode";

    // Read fullscreen flag
    cfg.loadBlock("display");
    cfg.getBool("fullscreen", &AppControl[FULLSCREEN]);
    if( AppControl[FULLSCREEN] )
        SetFullscreen();

    cfg.getFloat("planes", m_fPlanes, 2);
    cfg.getFloat("fovy",   &m_fFovY);

    // Setup misc items
    int iFontSize;
    cfg.getInt("font_size", &iFontSize);
    SetFont( iFontSize, "Times" );
    SetCursor( CRNone );
    
    // Misn opengl items
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable        (GL_CULL_FACE);
    glEnable        (GL_DEPTH_TEST);
    glShadeModel    (GL_SMOOTH);
    glPolygonMode   (GL_FRONT,GL_FILL);

    // Set recording's mode destination file
    cfg.loadBlock("global");

    cfg.getBool("collision", &AppControl[COLLISION]);

    string sOutputFile;
    cfg.getString("record_output", sOutputFile);
    Recorder::Instance().setFile( sOutputFile.c_str() );

    // Setup ambient lighting
    float gAmbient[4];
    cfg.getFloat("light", gAmbient, 4);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, gAmbient);

    // Initialize object's
    m_Minimap.init();
    ShaderMgr::Instance().init();
    ObjectMgr::Instance().init();
    LightMgr::Instance().init();

    // Setup the two cameras
    // Third person
    TPCamera *camStalker = CameraMgr::Instance().add<TPCamera>("stalker");
    camStalker->init( (Object*)&ObjectMgr::Instance().getShip() );
    // First person
    FPCamera *camFP = CameraMgr::Instance().add<FPCamera>("first-person");
    camFP->init( (Object*)&ObjectMgr::Instance().getShip() );
    
    CameraMgr::Instance().activate("first-person");
}

// ----------------------------------------------------------------------------
void MyWindow::OnDisplay()
{
    Clock::Instance().FrameStep();   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    OnResize(Width(), Height());
   
    // Apply camera
    CameraMgr::Instance().apply();
   
    // Render Lights
    LightMgr::Instance().render();
    // Render Scene
    ObjectMgr::Instance().render();
    //PSManager::Instance().render();
    // Interface and minimap
    drawInterface();

    SwapBuffers();
}

// ----------------------------------------------------------------------------
void MyWindow::OnIdle() 
{
    if( Recorder::Instance().isOn() ) {
        Recorder::Instance().record( Clock::Instance().getCurrentFeed() );
    }
    
	// Update spaceship and its shield
    ObjectMgr::Instance().update();
    LightMgr::Instance().update();
    // Update third-person camera
    //CameraMgr::Instance().getActive()->update();
    CameraMgr::Instance().get("stalker")->update();
    CameraMgr::Instance().get("first-person")->update();

    // --------------------------------------------------------------------
    if( AppControl[COLLISION] )
        ObjectMgr::Instance().checkCollision();

    // At last update particles and check collisions with the tiles and
    // with the object's within them
    PSManager::Instance().update();

    Redraw();
}

// ----------------------------------------------------------------------------
void MyWindow::OnResize(int w, int h) {
    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(w) / float(h);
    gluPerspective( m_fFovY, ratio, m_fPlanes[0], m_fPlanes[1] );
    m_fDimRatio = ratio;

    // Setup modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,w,h);
}

// ----------------------------------------------------------------------------
void MyWindow::OnDestroy() 
{   
    // Explicitly destroy singletons
    CameraMgr::Destroy();
    ObjectMgr::Destroy();
    PSManager::Destroy();
    Tilemap::Destroy();
    TextureMgr::Destroy();
    ShaderMgr::Destroy();
    LightMgr::Destroy();
    Clock::Destroy();
    Recorder::Destroy();
}

// ----------------------------------------------------------------------------
void MyWindow::OnKeyboard(int key, bool down)
{
	key = tolower(key);
	if( key == 'q' || key == 27 ) Close();

    // Wireframe switch
    if( key == AppControlKey[WIREFRAME] && !down ) {
        if( AppControl[WIREFRAME] ) {
            glPolygonMode(GL_FRONT,GL_FILL);
        } else {
            glPolygonMode(GL_FRONT,GL_LINE);
        }
        AppControl[WIREFRAME] = !AppControl[WIREFRAME];
    }

    // Record switch
    if( key == AppControlKey[RECORD] && !down ) {
        if( AppControl[RECORD] ) {
            Recorder::Instance().stop();
        } else {
            if( !Recorder::Instance().start() ) {
                _LOG("Failed to open output file for recording");
            }
        }
        AppControl[RECORD] = !AppControl[RECORD];
    }

    // Collision switch
    if( key == AppControlKey[COLLISION] && !down ) {
        AppControl[COLLISION] = !AppControl[COLLISION];
    }

    // Camera switch
    if( key == AppControlKey[CAMERA] && !down ) {
        if( AppControl[CAMERA] ) {
            CameraMgr::Instance().activate("first-person");
            ObjectMgr::Instance().hideShip();
        } else {
            CameraMgr::Instance().activate("stalker");
            ObjectMgr::Instance().showShip();
        }
        AppControl[CAMERA] = !AppControl[CAMERA];
    }

    // Capture keyboard for spaceship
    OCKeyboard *cShipCont = 
        (OCKeyboard*)ObjectMgr::Instance().getShip().getComponent("controller");
    cShipCont->capture( key, down );

    Redraw();
}	

/**
 * This function draws the user interface. Although the is not so 
 * much interfacing going, we like to call it that way, because it
 * is more natural.
 */
void MyWindow::drawInterface()
{
    // Disable lighting
    glDisable( GL_LIGHTING );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float fWidth  = m_fDimRatio;
    const float fHeight = 1.0f;
    gluOrtho2D( -(double)fWidth, (double)fWidth, 
                -(double)fHeight, (double)fHeight );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // A crossair for the first person mode
    if( ObjectMgr::Instance().isShipHidden() )
    {
        float fLineSize = 0.035f;
        glBegin(GL_LINES);
            glColor3f(1.0f,1.0f,1.0f);
            glVertex2f(.0f, fLineSize);
            glVertex2f(.0f, -fLineSize);
            glVertex2f(fLineSize, .0f);
            glVertex2f(-fLineSize, .0f);
        glEnd();
    }

    glPushMatrix();
    {
        const float fSizeRatio = 0.0005f;
        float fStationHalfSize = Tilemap::Instance().getTileSize() * 
                                 Tilemap::Instance().getNumOfTiles() * 0.5f;
        const float fSide = fStationHalfSize * fSizeRatio;
        //glScissor( Width()-int(fStationHalfSize*0.5f), 
        //           Height()-int(fStationHalfSize*0.5f), 
        //           Width(), 
        //           Height() );

        //glEnable(GL_SCISSOR_TEST);
        // Translate and rotate the minimap to our likes
        // [bottom-right corner]
        glTranslatef( fWidth-fSide, -fHeight+fSide, 0.0f );

        if( ObjectMgr::Instance().isShipHidden() )
        {
            // Draw the minimap background
            m_Minimap.setMinimapSize( fSide );
            m_Minimap.render();
            ObjectMgr::Instance().renderForMinimap( fSizeRatio );
        }

        // Disable scissor testing
        //glDisable(GL_SCISSOR_TEST);
    }
    glPopMatrix();

    // Draw screen text elements
    glColor3f( 1.0f, 1.0f, 0.0f );    
    const float fX = -fWidth+0.01f;
    const float fY = -fHeight+0.015f;
    const float fStep = 0.055f;
    char *flag;
    int kk;
    for( kk=0; kk<NUM_OF_CONTROLS; ++kk )
    {
        glRasterPos3f( fX, fY+kk*fStep, 0.0f );
        flag = "Off";
        if( AppControl[kk] ) flag = "On";
        Printf( "%s[%c]: %s", AppControlLabel[kk], AppControlKey[kk], flag );
    }

    int iHealth, iLives, iMaxLives;
    ObjectMgr::Instance().getShip().getVitals( iHealth, iLives, iMaxLives );
    glRasterPos3f( fX, fY+(kk++)*fStep, 0.0f );
    Printf( "Health: %i", iHealth );

    glRasterPos3f( fX, fY+(kk++)*fStep, 0.0f );
    Printf( "Lives: %i/%i", ((0==iLives)?0:--iLives), --iMaxLives );

    glEnable( GL_LIGHTING );
}