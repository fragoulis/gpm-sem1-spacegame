#include "TPCamera.h"
#include "QuatRotation.h"
#include "CollisionDynamicBBox.h"
#include "CollisionResponse.h"
#include "Movement.h"
#include "Config.h"

using namespace tlib;

TPCamera::TPCamera()
{}

void TPCamera::init( Object *oTarget )
{
    m_oTarget = oTarget;

    OCQuatRotation *cTarOri = (OCQuatRotation*)m_oTarget->getComponent("orientation");
    OCQuatRotation *cCamOri = (OCQuatRotation*)getComponent("orientation");

    Config cfg("config.txt");
    cfg.loadBlock("tpcamera");

    // Initialize position offset in respect to the target
    float vfOffset[3];
    cfg.getFloat("offset", vfOffset, 3);
    m_vPosOffset.xyz(vfOffset);

    float vfViewOffset[3];
    cfg.getFloat("viewpoint", vfViewOffset, 3);
    const Vector3f vViewOffset(vfViewOffset);

    // Position the camera
    m_vPos = m_oTarget->getPos() + m_vPosOffset;
    //std::cout << m_vPos << std::endl;
    // Calculate the view point by adding to the target's position
    // the offset
    Vector3f vViewPoint = m_oTarget->getPos() + vViewOffset;
    //std::cout << vViewPoint << std::endl;
    // Setup view vector
    Vector3f vView = vViewPoint - m_vPos;
    vView.normalize();
    cCamOri->setView( vView );

    // We know that the object's and camera's right vectors are 
    // going to be parallel
    cCamOri->setRight( cTarOri->getRight() );
    
    // We calculate up
    // Because view is negative we must turn up upside down
    // by multiplying with -1
    Vector3f vUp = cCamOri->getView().cross( cCamOri->getRight() ) * (-1);
    vUp.normalize();
    cCamOri->setUp( vUp );

    // Read camera's rotation and position bias
    cfg.getFloat("rotation_bias", &m_fRotationBias);
    cfg.getFloat("position_bias", &m_fPositionBias);

    // Read camera's bounding box
    float vfBBox[3];
    cfg.getFloat("bbox", vfBBox, 3);

    // Initialize collision component
    // [Here we give our object its bounding box]
    OCCollisionDynamicBBox *cCol = new OCCollisionDynamicBBox;
    cCol->setBBox( Vector3f( vfBBox ) );
    setComponent( cCol );

    // Initialize collision response component
    // [This is the simplest for of response, just position correction]
    setComponent( new IOCCollisionResponse );
}

/**
 * Updates the position of the camera
 */
void TPCamera::update()
{
    if( !m_oTarget ) return;

    OCQuatRotation *cTarOri = 
        (OCQuatRotation*)m_oTarget->getComponent("orientation");
    OCQuatRotation *cCamOri = (OCQuatRotation*)getComponent("orientation");

    Quatf qRot = cTarOri->getYaw() * cTarOri->getPitch();
    Quatf qRes;
    cCamOri->getRot().slerp( qRot, m_fRotationBias, qRes );
    cCamOri->setRot(qRes);
    // Apply to the vectors the same rotations that were 
    // applied to the spaceship
    // View
    cCamOri->getView().selfRotate( qRes );

    // Up
    cCamOri->getUp().selfRotate( qRes );
    
    // Save position
    m_vPrevPos = m_vPos;

    // Recalculate the position
    m_vPosOffset.selfRotate( qRes );
    m_vPos += ( m_oTarget->getPos() + m_vPosOffset - m_vPos ) * m_fPositionBias;

    // Reset the rotations of the orientation component
    cTarOri->resetAngles();
}