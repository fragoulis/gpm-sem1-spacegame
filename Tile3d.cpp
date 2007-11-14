#include "Tile3d.h"
#include "SpacestationCorridorsDisplayList.h"
#include "DoorMgr.h"
#include "Door.h"
#include "OutletMgr.h"
#include "Outlet.h"
#include "CollisionBBox.h"
#include "Config.h"
#include "Logger.h"

Tile3d::Tile3d(): m_oOccupant(0) {}

Tile3d::~Tile3d()
{
    if( m_oOccupant ) {
        delete m_oOccupant;
        m_oOccupant = 0;
    }
}

// ----------------------------------------------------------------------------
void Tile3d::addObject( int objType )
{
    switch( objType )
    {
    case OBJ_LIGHT: break;
    case OBJ_DOOR: addDoor(); break;
    case OBJ_OUTLET: addOutlet(); break;
    }

} // end addObject()

// ----------------------------------------------------------------------------
void Tile3d::addDoor()
{
    _LOG("Adding door at [" + toStr<int>(i) + toStr<int>(j) + toStr<int>(k) + "]" );

    // Allocate door object
    Door *door = new Door;
    
    // Set its position vector
    int ijk[] = { i, j, k };
    door->setPosFromIndex( ijk );
    
    // By default every door is aligned to the z-axis
    // Correct the door's orientation according to what tile its 
    // installed to
    // [x-axis,y-axis]
    Quatf qCorrectRotation;
    // We must also correct the bounding boxes orientations
    // To do this, we just resize each side to fit to the new orientation
    OCCollisionBBox *cBBox = (OCCollisionBBox*)door->getComponent("collision");
    const Vector3f& vBBox = cBBox->getBBox();

    switch( m_iCorrType ) {
        case TW_X_ALIGNED: 
            qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
            door->setDir( qCorrectRotation );
            cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
            break;
        case TW_Y_ALIGNED: 
            qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
            door->setDir( qCorrectRotation );
            cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
            break;
    }
    

    // Push the door to the manager
    DoorMgr::add(door);
    // Save it as this tile's occupant
    m_oOccupant = (Object*)door;
}

// ----------------------------------------------------------------------------
void Tile3d::addOutlet()
{
    _LOG("Adding outlet at [" + toStr<int>(i) + toStr<int>(j) + toStr<int>(k) + "]" );
    
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

    switch( m_iCorrType ) {
        case TW_X_ALIGNED: 
            qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Up() ); 
            ol->setDir( qCorrectRotation );
            cBBox->setBBox( Vector3f( vBBox.z(), vBBox.y(), vBBox.x() ) );
            vfNewOffset[2] = -vfOffset[0];
            break;
        case TW_Y_ALIGNED: 
            qCorrectRotation.toRotation( (float)M_PI_2, Vector3f::Right() );
            ol->setDir( qCorrectRotation );
            cBBox->setBBox( Vector3f( vBBox.x(), vBBox.z(), vBBox.y() ) );
            break;
    }

    // Set its position vector
    int ijk[] = { i, j, k };
    ol->setPosFromIndex( ijk, vfNewOffset );

    // Push the outlet to the manager
    OutletMgr::add(ol);
    // Save it as this tile's occupant
    m_oOccupant = (Object*)ol;
}