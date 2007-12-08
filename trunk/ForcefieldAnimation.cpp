#include "ForcefieldAnimation.h"
#include "Forcefield.h"
#include "Clock.h"
#include "Collision.h"
#include "Tilemap.h"
using namespace tlib;

void ForcefieldAnimation::onUpdate()
{
    // Decrease alpha
    ((Forcefield*)getOwner())->fade( Clock::Instance().getDeltaTime() );

} // end onUpdate()

bool ForcefieldAnimation::condition()
{   
    // Stop when surface becomes completely transparent
    if( ((Forcefield*)getOwner())->getAlpha() < 1e-6f )
        return true;

    return false;

} // end condition()

void ForcefieldAnimation::onStart()
{
    // When forcefield is deactivated the field is clear for the spaceship to 
    // cross, thus we turn of the detection with this bounding box by 
    // deactivating it
    IOCCollision *cCol = (IOCCollision*)getOwner()->getComponent("collision");
    cCol->deactivate();
}

void ForcefieldAnimation::onStop()
{
    // This object occupies a tile
    // We must set this tile's object pointer to null so that 
    // we can safely remove the object
    Tile3d* tile = Tilemap::Instance().getTile( getOwner()->getPos() );
    tile->setOccupant(0);
}