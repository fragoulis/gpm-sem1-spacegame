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

    Config cfg("config.txt");
    cfg.loadBlock("display");
    
    int iFullScreen;
    cfg.getInt("fullscreen", &iFullScreen);
    if( iFullScreen )
        SetFullscreen();

    SetFont( 30, "Times" );
    SetCursor( CRNone );

    cfg.getFloat("planes", m_fPlanes, 2);
    cfg.getFloat("fovy", &m_fFovY);

    glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
    //// enable backface culling
    glEnable        (GL_CULL_FACE);
    glEnable        (GL_DEPTH_TEST);
    glShadeModel    (GL_SMOOTH);
    glPolygonMode   (GL_FRONT,GL_FILL);

    //// ----SETUP SOME DEFAULT GLOBAL LIGHTING -------
    //glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    //glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

    //// Low ambient
    cfg.loadBlock("global");

    float gAmbient[4];
    cfg.getFloat("light", gAmbient, 4);
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, gAmbient);

    //// Open log file
    Logger::Instance().open( Logger::ERROR_LOG, true );

    ShaderMgr::Instance().init();
    ObjectMgr::Instance().init();
    LightMgr::Instance().init();

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
    gluPerspective( m_fFovY, ratio, m_fPlanes[0], m_fPlanes[1] );
    // reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,Width(),Height());

    CameraMgr::Instance().apply();
   
    // Render spacestation
    LightMgr::Instance().render();
    ObjectMgr::Instance().render();

    drawInterface();

    SwapBuffers();
}

void MyWindow::OnIdle() 
{
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
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float(w) / float(h);
    gluPerspective( 80, ratio, 0.9f, 1000.0f );
	glMatrixMode(GL_MODELVIEW);
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
    Logger::Destroy();
    ShaderMgr::Destroy();
    LightMgr::Destroy();
}

void MyWindow::OnKeyboard(int key, bool down)
{
    // Here we'll add, GOD MODE, CAMERA TOGGLE, RECORD MODE
	key = tolower(key);
    if( key == 'q' || key == 27 ) Close();

    if( key == 'l' && !down ) {
        static bool bIsLines = true;
        if( bIsLines ) {
            glPolygonMode(GL_FRONT,GL_LINE);
        } else {
            glPolygonMode(GL_FRONT,GL_FILL);
        }
        bIsLines = !bIsLines;
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
    double ratio = double(Width()) / double(Height());
    gluOrtho2D( -ratio, ratio, -1.0, 1.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Only update the minimap every N frames. Render on a frame bufffer[texture]
    // and reapply every N frames??
    float fStationHalfSize = Tilemap::Instance().getTileSize() * 
                             Tilemap::Instance().getNumOfTiles() * 0.5f;
    float fSizeRatio = 0.0005f;
    
    enum MiniColors { SPACESHIP, BARRIER, TURRET, REACTOR };
    float vfColors[][4] =
    {
        { 0.0f, 1.0f, 0.0f, 1.0f }, // Spaceship
        { 0.0f, 1.0f, 1.0f, 1.0f }, // Barrier
        { 1.0f, 0.0f, 0.0f, 1.0f }, // Turret
        { 1.0f, 1.0f, 1.0f, 1.0f }  // Reactor?
    };


    glPointSize(3.0f);
    glPushMatrix();
    {
        // Translate and rotate the minimap to our likes
        // [bottom-right corner]
        glTranslatef( 1.04f, -0.77f, 0.0f );
        glRotatef( 40, 1.0f, 0.0f, 0.0f );
        glRotatef( 30, 0.0f, 1.0f, 0.0f );
        
        glBegin(GL_POINTS);
        {
            // Translate the spaceship dor according to the real
            // spaceship's world coordinates
            const Vector3f& vShip = ObjectMgr::Instance().getShip().getPos() * fSizeRatio;
            glColor4fv( vfColors[SPACESHIP] );
            glVertex3f( vShip.x(), vShip.y(), vShip.z() );

            // Draw all objects in the tilemap
            for( int index=0; index<Tilemap::Instance().getArraySize(); ++index )
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
                        case Object::BARRIER: iColorIndex = BARRIER; break;
                        case Object::TURRET: iColorIndex = TURRET; break;
                    }
                    glColor4fv( vfColors[iColorIndex] );

                    // Position point
                    const Vector3f& vPos = oTile->getOccupant()->getPos() * fSizeRatio;
                    glVertex3f( vPos.x(), vPos.y(), vPos.z() );

                } // end if( oOcc ) ...

            } // end for( ... )
        }
        glEnd();

        // Save current polygon display mode state and turn on wireframe
        int viPolygonMode[2];
        glGetIntegerv( GL_POLYGON_MODE, viPolygonMode );
        glPolygonMode(GL_FRONT,GL_LINE);
        glPushMatrix();
        {
            // Make spacestation fSizeRatio times smaller
            glScalef( fSizeRatio, fSizeRatio, fSizeRatio );
            ((IOCVisual*)ObjectMgr::Instance().getCorridors().getComponent("visual"))->render();
        }
        glPopMatrix();
        // Restore polygon display modes
        glPolygonMode(GL_FRONT,viPolygonMode[0]);
    }
    glPopMatrix();

    // Draw health status
    glColor3f( 1.0f, 1.0f, 0.0f );
    glRasterPos3f( -1.5f, -0.9f, -0.1f );
    Printf( "Health: %i", ObjectMgr::Instance().getShip().getHealth() );
    
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