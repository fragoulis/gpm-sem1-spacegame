#include "DoorAnimation.h"
#include "Door.h"
#include "Movement.h"
#include "Collision.h"
#include "Tilemap.h"
using namespace tlib;

void DoorAnimation::onUpdate()
{
    DoorPanel *panel;
    IOCMovement *cMov;

    // Update owner
    // Here the owner is a door barrier
    // The door barrier itseld is not movable but the four door panels
    // comprising the door are
    // So, what we do is update the position of these four door panels    
    // left
    panel = ((Door*)getOwner())->leftPanel();
    cMov = (IOCMovement*)panel->getComponent("movement");
    cMov->update();
    // right
    panel = ((Door*)getOwner())->rightPanel();
    cMov = (IOCMovement*)panel->getComponent("movement");
    cMov->update();
    // top
    panel = ((Door*)getOwner())->topPanel();
    cMov = (IOCMovement*)panel->getComponent("movement");
    cMov->update();
    // bottom
    panel = ((Door*)getOwner())->bottomPanel();
    cMov = (IOCMovement*)panel->getComponent("movement");
    cMov->update();

} // end onUpdate()

bool DoorAnimation::condition()
{
    // The condition to stop the animation is to check how much a panel
    // has moved. It must stop the moment it has moved its x-dimension.
    // We are being deterministic here, as we check only for one panel, and 
    // take for granted that the other panels are on the same state
    float fPanelX = ((Door*)getOwner())->rightPanel()->getPos().x();

    if( m_fDistMoved > 2.0f * Door::PanelDim ) {
        return true;
    }

    m_fDistMoved += fPanelX - m_fPrevX;
    m_fPrevX = fPanelX;

    return false;

} // end condition()

void DoorAnimation::onStart()
{
    // When door starts opening the field in clear for the spaceship to 
    // cross, thus we turn of the detection with this bounding box by 
    // deactivating it
    IOCCollision *cDoorCol = (IOCCollision*)getOwner()->getComponent("collision");
    cDoorCol->deactivate();

    // Initialize the helper variables
    // Set distance door has moved to zero
    m_fDistMoved = 0.0f;
    // Set the previous position to the current one
    m_fPrevX = ((Door*)getOwner())->rightPanel()->getPos().x();
}

void DoorAnimation::onStop()
{
    // This object occupies a tile
    // We must set this tile's object pointer to null so that 
    // we can safely remove the object
    Tile3d* tile = Tilemap::Instance().getTile( getOwner()->getPos() );
    tile->setOccupant(0);
}