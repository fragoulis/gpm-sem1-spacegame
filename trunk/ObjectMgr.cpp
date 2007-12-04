#include <windows.h>
#include <gl/gl.h>
#include "ObjectMgr.h"
#include "Tile3d.h"
// Objects
#include "DoorMgr.h"
#include "Door.h"
#include "BladeMgr.h"
#include "RotatingBlade.h"
#include "ForcefieldMgr.h"
#include "Forcefield.h"
#include "OutletMgr.h"
#include "Outlet.h"
#include "TurretMgr.h"
#include "Turret.h"
// Components
#include "Visual.h"
#include "LinearMovement.h"
#include "CollisionDynamicBBox.h"
#include "CollisionDynamicBSphere.h"
#include "CollisionResponse.h"
// Singletons
#include "CameraMgr.h"
#include "Tilemap.h"
// Util
#include "Config.h"
#include "Logger.h"

namespace tlib
{

    ObjectMgr::ObjectMgr()
    {
        // Read culling distance from config file
        Config cfg( "config.txt" );
        cfg.loadBlock("global");
        cfg.getFloat("cull_distance", &m_fCullDistance);
    }

    ObjectMgr::~ObjectMgr()
    {}

    // ----------------------------------------------------------------------------
    void ObjectMgr::init()
    {
        m_Station.init();
        m_Corridors.init();
        m_Ship.init();
        m_Shield.init( &m_Ship );
        m_Reactor.init();
    }

    // ----------------------------------------------------------------------------
    void ObjectMgr::update()
    {
        m_Ship.update();
        m_Shield.update();
    }

    // ----------------------------------------------------------------------------
    void ObjectMgr::render()
    {
        m_Ship.applyLight();
        ((IOCVisual*)m_Station.getComponent("visual"))->render();
        //m_Reactor.render();
        //((IOCVisual*)m_Ship.getComponent("visual"))->render();
        //((IOCVisual*)m_Corridors.getComponent("visual"))->render();

        //glEnable( GL_BLEND );
        //glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        //((IOCVisual*)m_Shield.getComponent("visual"))->render();
        //glDisable( GL_BLEND );
    }

    // ----------------------------------------------------------------------------
    // ---- Collision detection course
    // 1.   Check if spaceship is within a tile
    // 2.   If it is,
    // 2.1  Test with corridors and objects
    // 2.2  Test camera with corridors
    // 3    If it's not test with spacestation
    // 3.1  If its inside spacestation test with reactor
    // 4    If its inside but was not before
    // 4.1  Check if its at the entrance
    void ObjectMgr::checkCollision()
    {
        // Save the ship's collision components
        OCCollisionDynamicBBox *cShipCol = 
            (OCCollisionDynamicBBox*)m_Ship.getComponent("collision");
        IOCCollisionResponse *cShipRes = 
            (IOCCollisionResponse*)m_Ship.getComponent("collisionresponse");

        // This variables will help us determine some of the collision
        // details
        static bool 
            bInStation = false,
            bInReactor = false;
        // The collision vector
        Vector3f vColDir;

        // Check if ship is in a tile
        if( cShipCol->readTile() ) {
            // Check spaceship with corridors and objects
            cShipCol->checkCollision();

            // Check camera with corridors
            Camera *camTP = CameraMgr::Instance().getActive();
            OCCollisionDynamicBBox *cCamCol = 
                (OCCollisionDynamicBBox*)camTP->getComponent("collision");
            if( cCamCol->readTile() ) {
                // Check spaceship with corridors and objects
                cCamCol->checkCollisionWithTile();
            }
            // This check helps us make sure that the camera will never 
            // go outside the corridor walls
            else if( Tilemap::Instance().getTile( camTP->getPrevPos() ) ) {
                camTP->setPos( camTP->getPrevPos() );
            }
        }
        else if( cShipCol->check( (Object*)&m_Station, vColDir ) )
        {
            if( !bInStation )
            {
                // Declare some shorthands
                // The ship's bounding box
                const Vector3f& bbox = ((OCCollisionBBox*)cShipCol)->getBBox();
                // The ship's position
                const Vector3f& pos = m_Ship.getPos();
                // The tilesize
                int iTileSize = Tilemap::Instance().getTileSize();

                // Here we are certainly somewhere inside the spacestation.
                // Although we only respond if we are not in the station's 
                // 'front door'.
                // Hence, we check against certain coordinates to figure out
                // whether the ship is at the gate.
                if( pos.x() > bbox.x() && pos.x()+bbox.x() < iTileSize ) {
                    if( pos.y() > bbox.y() && pos.y()+bbox.y() < iTileSize ) {
                        bInStation = true;
                    }
                }

                if( !bInStation )
                    cShipRes->respond( vColDir );
            } // end if( !bIsInside )...
            else {
                bInReactor = true;
                // Check spaceship with the reactor
                if( cShipCol->check( (Object*)&m_Reactor, vColDir ) ) {
                    cShipRes->respond( vColDir );
                }
            }

        } // end else if( .... )
        else {
            bInStation = false;
        }

    } // end checkCollision()

    // ----------------------------------------------------------------------------
    void ObjectMgr::addDoor( Tile3d *oTile )
    {
        _LOG("Adding door at [" + toStr<int>(oTile->i()) + " "
                                + toStr<int>(oTile->j()) + " "
                                + toStr<int>(oTile->k()) + "]" );

        // Allocate door object
        Door *door = new Door;
        
        // Set its position vector
        door->setPosFromIndex( oTile->ijk() );
        
        // By default every door is aligned to the z-axis
        // Correct the door's orientation according to what tile its 
        // installed to
        // [x-axis,y-axis]
        Quatf qCorrectRotation;
        // We must also correct the bounding boxes orientations
        // To do this, we just resize each side to fit to the new orientation
        OCCollisionBBox *cBBox = (OCCollisionBBox*)door->getComponent("collision");
        const Vector3f& vBBox = cBBox->getBBox();

        switch( oTile->getType() ) {
            case TW_X_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
                cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
                break;
            case TW_Y_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
                cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
                break;
        }
        door->setDir( qCorrectRotation );
        

        // Push the door to the manager
        DoorMgr::add(door);
        // Save it as this tile's occupant
        oTile->setOccupant( (Object*)door );

    } // end addDoor()

    // ----------------------------------------------------------------------------
    void ObjectMgr::addBlade( Tile3d *oTile )
    {
        _LOG("Adding blade at [" + toStr<int>(oTile->i()) + " "
                                 + toStr<int>(oTile->j()) + " "
                                 + toStr<int>(oTile->k()) + "]" );

        // Allocate blade object
        RotatingBlade *blade = new RotatingBlade;
        
        // Set its position vector
        blade->setPosFromIndex( oTile->ijk() );
        
        // By default every blade is aligned to the z-axis
        // Correct the blade's orientation according to what tile its 
        // installed to
        // [x-axis,y-axis]
        Quatf qCorrectRotation;
        // We must also correct the bounding boxes orientations
        // To do this, we just resize each side to fit to the new orientation
        OCCollisionBBox *cBBox = (OCCollisionBBox*)blade->getComponent("collision");
        const Vector3f& vBBox = cBBox->getBBox();

        // We must also correct the direction vector for the Y-aligned blades
        OCLinearMovement *cLM = (OCLinearMovement*)blade->getComponent("movement");

        switch( oTile->getType() ) {
            case TW_X_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
                cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
                break;
            case TW_Y_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
                cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
                cLM->setDir( Vector3f::Back() );
                break;
        }
        blade->setDir( qCorrectRotation );
        
        // Push the blade to the manager
        BladeMgr::add(blade);
        // Save it as this tile's occupant
        oTile->setOccupant( (Object*)blade );

    } // end addBlade()

    // ----------------------------------------------------------------------------
    void ObjectMgr::addForcefield( Tile3d *oTile )
    {
        _LOG("Adding forcefield at [" + toStr<int>(oTile->i()) + " "
                                      + toStr<int>(oTile->j()) + " "
                                      + toStr<int>(oTile->k()) + "]" );

        // Create object
        Forcefield *field = ForcefieldMgr::add( oTile );
        
        // By default every object is aligned to the z-axis
        // Correct the object's orientation according to what tile its 
        // installed to
        // [x-axis,y-axis]
        Quatf qCorrectRotation;
        // We must also correct the bounding boxes orientations
        // To do this, we just resize each side to fit to the new orientation
        OCCollisionBBox *cBBox = (OCCollisionBBox*)field->getComponent("collision");
        const Vector3f& vBBox = cBBox->getBBox();

        switch( oTile->getType() ) {
            case TW_X_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
                cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
                break;
            case TW_Y_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
                cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
                break;
        }
        field->setDir( qCorrectRotation );
    }

    // ----------------------------------------------------------------------------
    void ObjectMgr::addOutlet( Tile3d *oTile )
    {
        _LOG("Adding outlet at [" + toStr<int>(oTile->i()) + " "
                                  + toStr<int>(oTile->j()) + " "
                                  + toStr<int>(oTile->k()) + "]" );
        
        // Allocate outlet object
        Outlet *ol = new Outlet;
        
        // Get the power outlet's position offset from the centre of the tile
        float vfOffset[3];
        Config cfg("config.txt");
        cfg.loadBlock("outlet");
        cfg.getFloat("offset", vfOffset, 3);

        // By default every outlet is aligned to the z-axis
        // Correct the outlets's orientation according to what tile its 
        // installed to
        // [x-axis,y-axis]
        Quatf qCorrectRotation;
        // We must also correct the bounding boxes orientations and 
        // the position offset
        // To do this, we just resize each side to fit to the new orientation
        OCCollisionBBox *cBBox = (OCCollisionBBox*)ol->getComponent("collision");
        const Vector3f& vBBox = cBBox->getBBox();
        float vfNewOffset[3] = { vfOffset[0], vfOffset[1], vfOffset[2] };

        switch( oTile->getType() ) {
            case TW_X_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
                cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
                vfNewOffset[2] = -vfOffset[0];
                break;
            case TW_Y_ALIGNED: 
                qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
                cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
                break;
        }
        ol->setDir( qCorrectRotation );

        // Set its position vector
        ol->setPosFromIndex( oTile->ijk(), vfNewOffset );

        // Push the outlet to the manager
        OutletMgr::add(ol);
        // Save it as this tile's occupant
        oTile->setOccupant( (Object*)ol );
    }

    // ----------------------------------------------------------------------------
    void ObjectMgr::addTurret( Tile3d *oTile )
    {
        _LOG("Adding turret at [" + toStr<int>(oTile->i()) + " "
                                  + toStr<int>(oTile->j()) + " "
                                  + toStr<int>(oTile->k()) + "]" );

        // Allocate object
        Turret *tr = new Turret;
        
        // Get the power outlet's position offset from the centre of the tile
        float vfOffset[3];
        Config cfg("config.txt");
        cfg.loadBlock("turret");
        cfg.getFloat("offset", vfOffset, 3);

        // By default every outlet is aligned to the z-axis
        // Correct the outlets's orientation according to what tile its 
        // installed to
        // [x-axis,y-axis]
        Quatf qCorrectRotation;
        // We must also correct the bounding boxes orientations and 
        // the position offset
        // To do this, we just resize each side to fit to the new orientation
        float vfNewOffset[3] = { vfOffset[0], vfOffset[1], vfOffset[2] };

        switch( oTile->getType() ) {
            case TW_Y_ALIGNED: 
                //qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
                vfNewOffset[2] = -vfOffset[1];
                break;
        }
        tr->setDir( qCorrectRotation );

        // Set its position vector
        tr->setPosFromIndex( oTile->ijk(), vfNewOffset );
        tr->init();

        // Push the outlet to the manager
        TurretMgr::add(tr);
        // Save it as this tile's occupant
        oTile->setOccupant( (Object*)tr );
    }

    // ----------------------------------------------------------------------------
    bool ObjectMgr::isCulled( Object *obj )
    {
        // Shorthand the active camera object
        Camera *activeCam = CameraMgr::Instance().getActive();

        // Find the distance between the object and the camera
        // If distance is greater than the predefined minimun one
        // then deactivate the object
        const Vector3f vDist = obj->getPos() - activeCam->getPos();
        if( vDist.length() > m_fCullDistance ) 
        {
            obj->deactivate();
            return true;
        } else {
            obj->activate();
        }

        return false;
    }

} // end of namespace tlib