#include "TPCamera.h"
#include "QuatRotation.h"
#include "CollisionDynamicBBox.h"
#include "Config.h"

using namespace tlib;

TPCamera::TPCamera():
    m_fFollowBias(0.0f),
    m_bActive(0)
    {}

void TPCamera::setup( Object *oTarget )
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
    // its view vector times the offset scalar
    Vector3f vViewPoint = m_oTarget->getPos() + vViewOffset;
    //std::cout << vViewPoint << std::endl;
    // Setup view vector
    Vector3f vView = vViewPoint - m_vPos;
    vView.normalize();
    cCamOri->setView( vView );
    //std::cout << vView << std::endl;

    // We know that the object's and camera's right vectors are 
    // going to be parallel
    cCamOri->setRight( cTarOri->getRight() );
    
    // We calculate up
    // Because view is negative we must turn up upside down
    // by multiplying with -1
    Vector3f vUp = cCamOri->getView().cross( cCamOri->getRight() ) * (-1);
    vUp.normalize();
    cCamOri->setUp( vUp );
    //std::cout << vUp << std::endl;

    // Initialize collision component
    // [Here we give our object its bounding box]
    OCCollisionDynamicBBox *cCol = new OCCollisionDynamicBBox;
    cCol->setBBox( Vector3f( 1.0f, 1.0f, 1.0f ) );
    setComponent( cCol );
}

/**
 * Updates the position of the camera
 */
void TPCamera::update()
{
    if( !m_oTarget ) return;
    //if( !m_bActive ) return;

    OCQuatRotation *cTarOri = (OCQuatRotation*)m_oTarget->getComponent("orientation");
    OCQuatRotation *cCamOri = (OCQuatRotation*)getComponent("orientation");
    //std::cout << "\nLOOP" << std::endl;

    Quatf qRes = cTarOri->getYaw() * cTarOri->getPitch();
    //Quatf qRes;
    //cCamOri->getRot().slerp( qRot, m_fFollowBias, qRes );
    //cCamOri->setRot(qRes);
    
    // Apply to the vectors the same rotations that were 
    // applied to the spaceship
    // View
    Vector3f vView = cCamOri->getView();
    vView.selfRotate( qRes );
    cCamOri->setView( vView );
    //std::cout << vView << std::endl;
    // Up
    Vector3f vUp = cCamOri->getUp();
    vUp.selfRotate( qRes );
    cCamOri->setUp( vUp );
    //std::cout << vUp << std::endl;
    
    //
    //Vector3f vInvCamPos = m_vPos - m_oTarget->getPos();
    //vInvCamPos.selfRotate( qRes );
    //vInvCamPos.normalize();
    m_vPosOffset.selfRotate( qRes );
    m_vPos = m_oTarget->getPos() + m_vPosOffset;
    //m_vPos = m_oTarget->getPos() + vInvCamPos;
    //std::cout << m_vPos << std::endl;

    cTarOri->resetAngles();
}