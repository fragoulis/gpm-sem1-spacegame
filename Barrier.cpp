#include "Barrier.h"
#include "CollisionBBox.h"
using tlib::OCCollisionBBox;

Barrier::Barrier() 
{
    // Initialize a bounding box for all barriers
    setComponent( new OCCollisionBBox );
}

Barrier::Barrier( const Vector3f& vBBox )
{
    // Initialize a bounding box for all barriers
    setComponent( new OCCollisionBBox( vBBox ) );
}