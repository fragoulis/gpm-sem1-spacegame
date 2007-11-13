#include "Tile3d.h"
#include "SpacestationCorridorsDisplayList.h"
#include "DoorRenderer.h"
#include "Door.h"
#include "Logger.h"

Tile3d::Tile3d( int pi, int pj, int pk, 
                int pcorrType, int pobjType ): obj(0)
{
    i = pi;
    j = pj;
    k = pk;
    corrType = pcorrType;
    addObject( pobjType );
}

Tile3d::~Tile3d()
{
    if( obj ) {
        delete obj;
        obj = 0;
    }
}

void Tile3d::addObject( int objType )
{
    int ijk[] = { i, j, k };

    switch( objType )
    {
    case OBJ_LIGHT: break;
    case OBJ_DOOR: 
        _LOG("Adding door...");
        // Allocate door object
        Door *door = new Door;
        // Set its position vector
        door->setPosFromIndex( ijk );
        // Save it as this tile's occupant
        obj = (Object*)door;
        // Push the door to the renderer
        DoorRenderer::add(door);
        break;
    }
}
