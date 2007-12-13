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
#include "ClockTimeSource.h"
#include "FileTimeSource.h"
// Components
#include "SpaceshipKeyboard.h"
#include "QuatRotation.h"
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
#include "EventMgr.h"
#include "Autopilot.h"

using namespace std;
using namespace tlib;

extern bool g_bSwitchCam	= false;
extern bool g_bToggleRecord = false;

MyWindow::MyWindow() 
{   
    Config cfg("config.txt");
    cfg.loadBlock("display");
    
    int iDim[2];
    cfg.getInt("dimensions", iDim, 2);
    SetSize( iDim[0], iDim[1] );

    SetPosition(100,100);

    m_isAppCreated = false;
}

// ----------------------------------------------------------------------------
void MyWindow::OnCreate() 
{
    // Safety flag, OnCreate() may be called more than once
    if( m_isAppCreated ) return;
    m_isAppCreated = true;

    GLWindowEx::OnCreate();
    _CLEAR_LOG

    Config cfg("config.txt");
    cfg.loadBlock("global");
    // Read whether this is a replay game
    bool bIsReplay;
    cfg.getBool("replay", &bIsReplay);

    // Read the file to output the recording to
    string sOutputFile;
    cfg.getString("record_output", sOutputFile);

    Recorder::Instance().setFiles( sOutputFile.c_str() );

    // Start the application clock.
    // If this is a replay game, use times from a file
    if( bIsReplay ) {
        if( !Recorder::Instance().openInputsForReplay() ) {
            _LOG("Failed to open replay files. Exiting!");
            Close();
        }
        Clock::Instance().Start( new FileTimeSource( sOutputFile.c_str() ) );
    } else {
        Clock::Instance().Start( new ClockTimeSource );
        
    }

    // Setup default ambient lighting
    float gAmbient[4];
    cfg.getFloat("light", gAmbient, 4);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, gAmbient);

    // Zero application bool control values
    memset( AppControl, 0, sizeof(AppControl) );
    // Read default collision checking state
    cfg.getBool("collision", &AppControl[COLLISION]);

    // Read application's control buttons
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

    // Read projection parametres
    cfg.getFloat("planes", m_fPlanes, 2);
    cfg.getFloat("fovy",   &m_fFovY);

    // Setup misc items
    int iFontSize;
    cfg.getInt("font_size", &iFontSize);
    SetFont( iFontSize, "Arial Bold" );
    SetCursor( CRNone );
    
    // Misn opengl items
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable        (GL_CULL_FACE);
    glEnable        (GL_DEPTH_TEST);
    glShadeModel    (GL_SMOOTH);
    glPolygonMode   (GL_FRONT,GL_FILL);

    // Initialize object's
    m_Minimap.init();
    ShaderMgr::Instance().init();
    ObjectMgr::Instance().init();
    LightMgr::Instance().init();

    // Setup the cameras
    // Third person
    TPCamera *camStalker = CameraMgr::Instance().add<TPCamera>("stalker");
    camStalker->init( (Object*)&ObjectMgr::Instance().getShip() );
    // First person
    FPCamera *camFP = CameraMgr::Instance().add<FPCamera>("first-person");
    camFP->init( (Object*)&ObjectMgr::Instance().getShip() );
    
    // Panoramic for final scene
    float vfCamPos[3];
    cfg.loadBlock("pan_cam");
    cfg.getFloat("position", vfCamPos, 3);

    Camera *camPan = CameraMgr::Instance().add<Camera>("pan-cam");
    camPan->setPos( Vector3f( vfCamPos ) );
    OCQuatRotation *cCamRot = (OCQuatRotation*)camPan->getComponent("orientation");
    cCamRot->lookAt( Vector3f(0.0f,0.0f,0.0f) );
    
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
	EventMgr::Instance().update();
	AppControl[CAMERA] = g_bSwitchCam;
    AppControl[RECORD] = g_bToggleRecord;

	// At the end of replay exit gracefully
	if( Recorder::Instance().isReplayOn() && 
        !Clock::Instance().getCurrentFeed() ) {
		Close();
	}
    
    if( Autopilot::Instance().isActive() ) {
        Autopilot::Instance().update();
    }
    else // Autopilot and recording cannot be active at the same time
	// If recording is on, save the time values
    if( Recorder::Instance().isRecordOn() ) {
        Recorder::Instance().record( Clock::Instance().getCurrentFeed() );
    }
    
	// Update spaceship and its shield
    ObjectMgr::Instance().update();
    LightMgr::Instance().update();
	// Update the cameras
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
    EventMgr::Destroy();
    Autopilot::Destroy();
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

    // Fullscreen switch
    if( key == AppControlKey[FULLSCREEN] && !down ) {
        if( AppControl[FULLSCREEN] ) {
            SetFullscreen(false);
        } else {
            SetFullscreen(true);
        }
        AppControl[FULLSCREEN] = !AppControl[FULLSCREEN];
    }

    // Collision switch
    if( key == AppControlKey[COLLISION] && !down ) {
        AppControl[COLLISION] = !AppControl[COLLISION];
    }

    // Capture keyboard for spaceship
    OCKeyboard *cShipCont = 
        (OCKeyboard*)ObjectMgr::Instance().getShip().getComponent("controller");
	if( cShipCont )
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
    glColor3f( 1.0f, 1.0f, 1.0f );
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

	glColor3f( 0.0f, 1.0f, 0.0f );
    int iHealth, iLives, iMaxLives;
    ObjectMgr::Instance().getShip().getVitals( iHealth, iLives, iMaxLives );
    glRasterPos3f( fX, fY+(kk++)*fStep, 0.0f );
    Printf( "Health: %i", iHealth );

    glRasterPos3f( fX, fY+(kk++)*fStep, 0.0f );
    Printf( "Lives: %i/%i", ((0==iLives)?0:--iLives), --iMaxLives );

    glEnable( GL_LIGHTING );
}