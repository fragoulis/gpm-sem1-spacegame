#include "Barrier.h"
#include "CollisionBBox.h"
using tlib::OCCollisionBBox;

Barrier::Barrier() 
{
    // Set object's type
    setType( BARRIER );

    // Initialize a bounding box for all barriers
    setComponent( new OCCollisionBBox );
}

Barrier::Barrier( const Vector3f& vBBox )
{
    // Set object's type
    setType( BARRIER );

    // Initialize a bounding box for all barriers
    setComponent( new OCCollisionBBox( vBBox ) );
}