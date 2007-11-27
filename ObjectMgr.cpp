#include "ObjectMgr.h"
#include "Tile3d.h"

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

#include "CollisionBBox.h"
#include "LinearMovement.h"
#include "Config.h"
#include "Logger.h"
using namespace tlib;

ObjectMgr::ObjectMgr()
{}

ObjectMgr::~ObjectMgr()
{}

// ----------------------------------------------------------------------------
void ObjectMgr::update()
{}

// ----------------------------------------------------------------------------
void ObjectMgr::render()
{}

// ----------------------------------------------------------------------------
void ObjectMgr::addDoor( Tile3d *oTile )
{
    _LOG("Adding door at [" + toStr<int>(oTile->i()) + 
                              toStr<int>(oTile->j()) + 
                              toStr<int>(oTile->k()) + "]" );

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
    _LOG("Adding blade at [" + toStr<int>(oTile->i()) + 
                              toStr<int>(oTile->j()) + 
                              toStr<int>(oTile->k()) + "]" );

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
    _LOG("Adding forcefield at [" + toStr<int>(oTile->i()) + 
                              toStr<int>(oTile->j()) + 
                              toStr<int>(oTile->k()) + "]" );

    // Allocate door object
    Forcefield *field = new Forcefield;
    
    // Set its position vector
    field->setPosFromIndex( oTile->ijk() );
    
    // By default every door is aligned to the z-axis
    // Correct the door's orientation according to what tile its 
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
    

    // Push the field to the manager
    ForcefieldMgr::add(field);
    // Save it as this tile's occupant
    oTile->setOccupant( (Object*)field );
}

// ----------------------------------------------------------------------------
void ObjectMgr::addOutlet( Tile3d *oTile )
{
    _LOG("Adding outlet at [" + toStr<int>(oTile->i()) + 
                              toStr<int>(oTile->j()) + 
                              toStr<int>(oTile->k()) + "]" );
    
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
    _LOG("Adding turret at [" + toStr<int>(oTile->i()) + 
                              toStr<int>(oTile->j()) + 
                              toStr<int>(oTile->k()) + "]" );

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
            qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
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