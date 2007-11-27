#include "ForcefieldAnimation.h"
#include "Forcefield.h"
#include "SimpleMaterial.h"
#include "Movement.h"
#include "Collision.h"
using namespace tlib;

void ForcefieldAnimation::onUpdate()
{
    OCSimpleMaterial *cMat = (OCSimpleMaterial*)getOwner()->getComponent("material");

    // Decrease alpha
    const float *rgba = cMat->getDiffuse().rgba();
    float fAlpha = rgba[3];
    fAlpha -= ((Forcefield*)getOwner())->getTTFactor() * IOCMovement::DeltaTime();

    // Update material
    Color newDiffuse = cMat->getDiffuse();
    newDiffuse.Assign( rgba[0], rgba[1], rgba[2], fAlpha );
    cMat->setDiffuse( newDiffuse );

} // end onUpdate()

bool ForcefieldAnimation::condition()
{
    OCSimpleMaterial *cMat = (OCSimpleMaterial*)getOwner()->getComponent("material");
    
    // Stop when surface becomes completely transparent
    if( cMat->getDiffuse().rgba()[3] < 1e-6f )
        return true;

    return false;

} // end condition()

void ForcefieldAnimation::onStart()
{
    // When door starts opening the field in clear for the spaceship to 
    // cross, thus we turn of the detection with this bounding box by 
    // deactivating it
    IOCCollision *cDoorCol = (IOCCollision*)getOwner()->getComponent("collision");
    cDoorCol->deactivate();
}

void ForcefieldAnimation::onStop()
{
    getOwner()->deactivate();
}