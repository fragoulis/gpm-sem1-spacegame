#include "Autopilot.h"
#include "VisualVertexArraySphere.h"
#include "LinearMovement.h"
#include "QuatRotation.h"
#include "ObjectMgr.h"
#include "Tilemap.h"
#include "Clock.h"
#include "Config.h"
using tlib::Config;
using tlib::OCVisualVertexArraySphere;

Autopilot::Autopilot():
m_uiCurWPoint(0),
m_bIsActive(false)
{
    for( int i=0; i<MAX_WPOINTS; ++i ) 
        m_WPoints[i].deactivate();

    Config cfg("config.txt");
    cfg.loadBlock("autopilot");
    cfg.getFloat("ship_end_pos", m_vfShipEndPos, 3);
}

    // if we are above the height of the reactof
    //   return nearest way point
    //   push nearest waypoint
    // push exitdoor waypoint
    // push far space waypoint
    // activate camera to show spacestation
// ----------------------------------------------------------------------------
void Autopilot::create()
{
    unsigned int wpc = 0; // waypoint counter
    Quatf qRot;

    Spaceship& ship = ObjectMgr::Instance().getShip();
    ship.deactivate();
    // Set the firstway point as the ship's current position
    m_WPoints[wpc].setPos( ship.getPos() );
    m_WPoints[wpc].setDir( ship.getDir() );
    m_WPoints[wpc++].activate();

    const Reactor& reactor = ObjectMgr::Instance().getReactor();
    if( ship.getPos().y() < reactor.getPos().y() )
    {
        // Get reactor's radius
        OCVisualVertexArraySphere *cReactorVisual = 
            (OCVisualVertexArraySphere*)reactor.getComponent("visual");

        const float fWPDist = cReactorVisual->getRadius() + 40.0f;

        // Create waypoints points around the reactor
        const unsigned int REACTOR_WAYPOINTS = 8;
        Vector3f vWPoints[REACTOR_WAYPOINTS];
        vWPoints[0].xyz( -fWPDist, 0.0f,     0.0f );
        vWPoints[1].xyz( -fWPDist, 0.0f, -fWPDist );
        vWPoints[2].xyz(     0.0f, 0.0f, -fWPDist );
        vWPoints[3].xyz(  fWPDist, 0.0f, -fWPDist );
        vWPoints[4].xyz(  fWPDist, 0.0f,     0.0f );
        vWPoints[5].xyz(  fWPDist, 0.0f,  fWPDist );
        vWPoints[6].xyz(     0.0f, 0.0f,  fWPDist );
        vWPoints[7].xyz( -fWPDist, 0.0f,  fWPDist );

        // Find the distances between these points and the spaceship
        // while keeping track of the smallest distance
        float vfDist[REACTOR_WAYPOINTS];
        unsigned int uiShortest = 0;
        vfDist[0] = ( vWPoints[0] - ship.getPos() ).length();
        for( int i=1; i<REACTOR_WAYPOINTS; ++i ) 
        {
            vfDist[i] = ( vWPoints[i] - ship.getPos() ).length();
            if( vfDist[i] < vfDist[uiShortest] )
            {
                uiShortest = i;
            }
        }

        m_WPoints[wpc].setPos( vWPoints[uiShortest] );
        qRot.toRotation( (float)M_PI_4, Vector3f::Right() );
        qRot.normalize();
        m_WPoints[wpc].setDir( qRot );
        m_WPoints[wpc++].activate();

    } // end if( .... )

    // Set the last waypoints
    m_WPoints[wpc].setPos( 
        Tilemap::Instance().getTilePos( 
            Tilemap::Instance().getExitDoorTile() 
            ) 
        );
    qRot.toRotation( (float)M_PI_2, Vector3f::Right() );
    qRot.normalize();
    m_WPoints[wpc].setDir( qRot );
    m_WPoints[wpc++].activate();
    
    m_WPoints[wpc].setPos( Vector3f(m_vfShipEndPos) );
    m_WPoints[wpc].setDir( qRot );
    m_WPoints[wpc].activate();
} // end create()

// ----------------------------------------------------------------------------
void Autopilot::update()
{
    static bool bChanged = false;

    Spaceship& ship = ObjectMgr::Instance().getShip();
    // Give a constant muximum speed
    ship.speed(1.0f);

    // Find direction vector[next waypoint - current waypoint]
    const unsigned int m_uiNextWPoint = m_uiCurWPoint + 1;
    if( m_uiNextWPoint == MAX_WPOINTS ) {
        return;
    }

    // ________________________________________________________________________
    const Waypoint 
        &nextWP = m_WPoints[m_uiNextWPoint],
        &curWP = m_WPoints[m_uiCurWPoint];

    const Vector3f vWPointDiff = nextWP.getPos() - curWP.getPos();
    // 
    Vector3f vDestDir;
    vWPointDiff.normalize( vDestDir );
    //
    const float fDist = vWPointDiff.length();
    
    // ________________________________________________________________________
    Vector3f vCurPosDiff = nextWP.getPos() - ship.getPos();
    //
    const float fCurDist = vCurPosDiff.length();

    // ________________________________________________________________________
    const float fBias = 1.0f - fCurDist / fDist;

    // ________________________________________________________________________
    // Update the ship's diretion and orientation vectors
    OCQuatRotation *cRot = (OCQuatRotation*)ship.getComponent("orientation");
    Quatf qRes;
    ship.getDir().slerp( nextWP.getDir(), fBias, qRes );
    ship.setDir( qRes );
    //cRot->setRot( qRes );
    //cRot->updateView( qRes );
    //cRot->updateUp( qRes );
    //cRot->updateRight();
    //cRot->setView( vDestDir );

    OCLinearMovement *cLM = (OCLinearMovement*)ship.getComponent("movement");
    cLM->setDir( vDestDir );
    //cLM->getDir().selfRotate( qRes );
    //cLM->setDir( cRot->getView() );

    //const Vector3f &vCurDir = cLM->getDir();

    //float dot = vDestDir.dot( vCurDir );
    //if( dot < -1.0f || dot > 1.0f ) return;
    //const float phi = acos(dot);
    //Vector3f normal = vDestDir.cross( vCurDir );
    //normal.normalize();

    //// --------------------------------------
    //Quatf qRot( phi, normal );
    //qRot.toRotation();
    //Quatf qRes;
    //OCQuatRotation *cRot = (OCQuatRotation*)ship.getComponent("orientation");

    //Vector3f viewTest = vCurDir.rotate( qRot );

    //dot = viewTest.dot( vDestDir );
    //if( fabs(dot) < 1.0f ) {
    //    qRot.toRotation( -phi, normal );   
    //}
    //cRot->getRot().slerp( qRot, fBias, qRes );

    //cRot->setRot( qRes );
    //cRot->getView().selfRotate( qRes );
    //cRot->getUp().selfRotate( qRes );

    //// If the dot product is near to -1, it means we look the wrong way.
    //// To correct this, we just rotate pi radians (a half circle).
    //if( dot < -0.999f ) {
    //    Quatf qCor = cRot->addYaw( (float)M_PI );
    //    cRot->updateView( qCor );
    //}

    //cRot->updateRight();
    //cLM->setDir( cRot->getView() );

    if( fBias > 0.99f ) ++m_uiCurWPoint;
} // end update()