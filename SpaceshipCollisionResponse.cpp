#include "SpaceshipCollisionResponse.h"
#include "Object.h"

void SpaceshipCollisionResponse::respond( const Vector3f &vCollVector ) const
{
    //((OCLinearMovement*)cShipMov)->setVelocity( -((OCLinearMovement*)cShipMov)->getVelocity() );
    m_oOwner->setPos( m_oOwner->getPos() + vCollVector );
}