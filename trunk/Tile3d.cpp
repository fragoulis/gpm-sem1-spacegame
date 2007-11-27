#include "Tile3d.h"
#include "ObjectMgr.h"
#include "Object.h"

Tile3d::Tile3d(): 
m_oOccupant(0) 
{}

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
    case OBJ_LIGHT: 
        break;

    case OBJ_DOOR: 
        ObjectMgr::Instance().addDoor( this ); 
        break;

    case OBJ_BLADE: 
        ObjectMgr::Instance().addBlade( this ); 
        break;

    case OBJ_FIELD: 
        ObjectMgr::Instance().addForcefield( this ); 
        break;

    case OBJ_OUTLET: 
        ObjectMgr::Instance().addOutlet( this ); 
        break;

    case OBJ_TURRET: 
        ObjectMgr::Instance().addTurret( this ); 
        break;
    }

} // end addObject()