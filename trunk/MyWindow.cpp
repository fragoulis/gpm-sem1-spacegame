#include "MyWindow.h"
// System
#include <iostream>
#include <cstdlib>
// Misc
#include "Config.h"
#include "Math.h"
// Objects
#include "TPCamera.h"
//#include "PSSmallExplosion.h"
//#include "PSSmoke.h"
// Components
#include "Movement.h"
#include "SpaceshipKeyboard.h"
#include "Visual.h"
#include "Shader.h"
// Singletons
#include "CameraMgr.h"
#include "ObjectMgr.h"
#include "ParticleSystemMgr.h"
#include "Tilemap.h"
#include "Logger.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"
#include "LightMgr.h"
#include "PointLight.h"

#include "Clock.h"
#include "Recorder.h"
#include "ClockTimeSource.h"
#include "FileTimeSource.h"

using namespace std;
using namespace tlib;

MyWindow::MyWindow() 
{   
    Config cfg("config.txt");
    cfg.loadBlock("display");
    
    int iDim[2];
    cfg.getInt("dimensions", iDim, 2);
    SetSize( iDim[0], iDim[1] );

    SetPosition(100,100);
}

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
    string sOutputFile;
    cfg.getString("record_output", sOutputFile);
    Recorder::Instance().setFile( sOutputFile.c_str() );

    // Setup ambient lighting
    float gAmbient[4];
    cfg.getFloat("light", gAmbient, 4);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, gAmbient);

    // Initialize object's
    ShaderMgr::Instance().init();
    ObjectMgr::Instance().init();
    LightMgr::Instance().init();

    // Setup the two cameras
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
    Clock::Instance().FrameStep();   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    OnResize(Width(), Height());
   
    // Apply camera
    CameraMgr::Instance().apply();
   
    // Render spacestation
    LightMgr::Instance().render();
    ObjectMgr::Instance().render();
    drawInterface();

    SwapBuffers();
}

void MyWindow::OnIdle() 
{
    if( Recorder::Instance().isOn() ) {
        Recorder::Instance().record( Clock::Instance().getCurrentFeed() );
    }

	// Update spaceship and its shield
    ObjectMgr::Instance().update();
    LightMgr::Instance().update();
    // Update third-person camera
    TPCamera *camTP = (TPCamera*)CameraMgr::Instance().get("stalker");
    camTP->update();

    // --------------------------------------------------------------------
    ObjectMgr::Instance().checkCollision();
    // At last update particles and check collisions with the tiles and
    // with the object's within them
    PSManager::Instance().update();

    Redraw();
}

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

void MyWindow::OnDestroy() 
{   
    // Explicitly destroy singletons
    ObjectMgr::Destroy();
    TextureMgr::Destroy();
    PSManager::Destroy();
    Tilemap::Destroy();
    CameraMgr::Destroy();
    ShaderMgr::Destroy();
    LightMgr::Destroy();
    Clock::Destroy();
    Recorder::Destroy();
}

void MyWindow::OnKeyboard(int key, bool down)
{
    // Here we'll add, GOD MODE, CAMERA TOGGLE, RECORD MODE
	key = tolower(key);
	if( key == 'q' || key == 27 ) Close();

    if( key == AppControlKey[WIREFRAME] && !down ) {
        if( AppControl[WIREFRAME] ) {
            glPolygonMode(GL_FRONT,GL_FILL);
        } else {
            glPolygonMode(GL_FRONT,GL_LINE);
        }
        AppControl[WIREFRAME] = !AppControl[WIREFRAME];
    }

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
    
    // Only update the minimap every N frames. Render on a frame bufffer[texture]
    // and reapply every N frames??
    
    enum MiniColors { SPACESTATION, SPACESHIP, BARRIER, TURRET, REACTOR };
    const float vfColors[][4] =
    {
        { 0.0f, 1.0f, 0.0f, 1.0f }, // Spacestation
        { 0.0f, 1.0f, 0.0f, 1.0f }, // Spaceship
        { 0.0f, 1.0f, 1.0f, 1.0f }, // Barrier
        { 1.0f, 0.0f, 0.0f, 1.0f }, // Turret
        { 0.0f, 0.0f, 1.0f, 1.0f }  // Reactor?
    };

    glPointSize(5.0f);
    glPushMatrix();
    {
        const float fSizeRatio = 0.0005f;
        float fStationHalfSize = Tilemap::Instance().getTileSize() * 
                                 Tilemap::Instance().getNumOfTiles() * 0.5f;
        const float fSide = fStationHalfSize * fSizeRatio;
        //glScissor( Width()-int(fStationHalfSize*0.5f), 0, Width(), Height() );
        //glEnable(GL_SCISSOR_TEST);
        // Translate and rotate the minimap to our likes
        // [bottom-right corner]
        glTranslatef( fWidth-fSide, -fHeight+fSide, 0.0f );

        // Draw the minimap background
        glPushMatrix();
        {
            glEnable(GL_TEXTURE_2D);
            static GLuint uiMinimap = TextureMgr::Instance().getTexture("textures/minimap2.jpg");
            glBindTexture(GL_TEXTURE_2D, uiMinimap );
            glEnable(GL_BLEND);
            glBegin(GL_QUADS);
            {
                glColor4f( 1.0f, 1.0f, 1.0f, 0.7f );
                glTexCoord2f(0,0); glVertex2f( -fSide, -fSide );
                glTexCoord2f(1,0); glVertex2f(  fSide, -fSide );
                glTexCoord2f(1,1); glVertex2f(  fSide,  fSide );
                glTexCoord2f(0,1); glVertex2f( -fSide,  fSide );
            }
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }
        glPopMatrix();

        glPushMatrix();
        {
            // Change depth func in order to always draw the minimap and the 
            // individual points on top of everything
            int iDepthFunc;
            glGetIntegerv(GL_DEPTH_FUNC, &iDepthFunc);
            glDepthFunc(GL_ALWAYS);

            // Correct the minimap position
            glTranslatef( 0.04f, -0.09f, 0.0f );
            glRotatef( 40, 1.0f, 0.0f, 0.0f );
            glRotatef( 30, 0.0f, 1.0f, 0.0f );

            // Save current polygon display mode state and turn on wireframe
            int viPolygonMode[2];
            glGetIntegerv( GL_POLYGON_MODE, viPolygonMode );
            glPolygonMode(GL_FRONT,GL_LINE);
            // Render small spacestation corridors
            glPushMatrix();
            {
                // Make spacestation fSizeRatio times smaller
                glColor4fv( vfColors[SPACESTATION] );
                glScalef( fSizeRatio, fSizeRatio, fSizeRatio );
                ((IOCVisual*)ObjectMgr::Instance().getCorridors().getComponent("visual"))->render();
            }
            glPopMatrix();
            // Render small reactor
            glPushMatrix();
            {
                // Make reactor fSizeRatio times smaller
                glColor4fv( vfColors[REACTOR] );
                const Vector3f& vPos = ObjectMgr::Instance().getReactor().getPos() * fSizeRatio;
                glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                glScalef( fSizeRatio*2.0f, fSizeRatio*2.0f, fSizeRatio*2.0f );
                ((IOCVisual*)ObjectMgr::Instance().getReactor().getComponent("visual"))->render();
            }
            glPopMatrix();

            // Restore polygon display modes
            glPolygonMode(GL_FRONT,viPolygonMode[0]);

            // Render object as small points
            glBegin(GL_POINTS);
            {
                // Translate the spaceship point according to the real
                // spaceship's world coordinates
                const Vector3f& vShip = ObjectMgr::Instance().getShip().getPos() * fSizeRatio;
                glColor4fv( vfColors[SPACESHIP] );
                glVertex3f( vShip.x(), vShip.y(), vShip.z() );

                // Draw all objects in the tilemap
                const int iArraySize = Tilemap::Instance().getArraySize();
                for( int index=0; index<iArraySize; ++index )
                {
                    Tile3d *oTile = Tilemap::Instance().getTileByIndex( index );
                    Object *oOcc = oTile->getOccupant();
                    if( oOcc )
                    {
                        if( oOcc->getType() == Object::LIGHT || 
                            oOcc->getType() == Object::OUTLET ) continue;

                        // Determine the type of the object in order to color the
                        // dor accordingly
                        int iColorIndex;
                        switch( oOcc->getType() ) {
                            case Object::BARRIER:   iColorIndex = BARRIER; break;
                            case Object::TURRET:    iColorIndex = TURRET; break;
                        }
                        glColor4fv( vfColors[iColorIndex] );

                        // Position point
                        const Vector3f& vPos = oTile->getOccupant()->getPos() * fSizeRatio;
                        glVertex3f( vPos.x(), vPos.y(), vPos.z() );

                    } // end if( oOcc ) ...

                } // end for( ... )
            }
            glEnd();

            // Restore depth test function
            glDepthFunc(iDepthFunc);
        }
        glPopMatrix();
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
    Printf( "Lives: %i/%i", --iLives, --iMaxLives );

    glEnable( GL_LIGHTING );
}

GLuint MyWindow::makeGrid( int quads, float size )
{
    int id = glGenLists(1);

    glNewList(id,GL_COMPILE);
    for(int i=0; i<quads; ++i)
    {
        for( int k=0; k<quads; ++k)
        {
            glPushMatrix();
            glTranslatef( float(i*size), float(k*size), 0.0f );
            glBegin(GL_QUADS);
                glNormal3f(0,0,1);
                glTexCoord2f(0,0); glVertex2f( 0,    0);
                glTexCoord2f(1,0); glVertex2f( size, 0);
                glTexCoord2f(1,1); glVertex2f( size, size );
                glTexCoord2f(0,1); glVertex2f( 0,    size );
            glEnd();
            glPopMatrix();
        }
    }
    glEndList();

     //glUseProgram(program);
    //glPushMatrix();
    //    //glEnable(GL_TEXTURE_2D);
    //    //glBindTexture( GL_TEXTURE_2D, 4 );
    //    //glUniform1i( ShaderMgr::Instance().getUniform(program,"colorMap"), 0 );
    //    //glRotatef( 90, 1.0f, 0.0f, 0.0f );
    //    glTranslatef( 0,0,720 );
    //    GLfloat mat_diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
    //    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //    glMaterialf(GL_FRONT, GL_SHININESS, 5.0f);
    //    glCallList(gridlist);
    //glPopMatrix();
    //glUseProgram(0);

    return id;
}