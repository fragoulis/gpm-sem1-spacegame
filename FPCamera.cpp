#include "FPCamera.h"
#include "QuatRotation.h"
#include "Config.h"
using tlib::Config;
using tlib::OCQuatRotation;

FPCamera::FPCamera():
m_oTarget(0)
{}

// ----------------------------------------------------------------------------
void FPCamera::init( Object *oTarget )
{
    m_oTarget = oTarget;

    Config cfg("config.txt");
    cfg.loadBlock("fpcamera");

    // Initialize position offset in respect to the target
    float vfOffset[3];
    cfg.getFloat("offset", vfOffset, 3);
    m_vPosOffset.xyz(vfOffset);   
}

// ----------------------------------------------------------------------------
void FPCamera::update()
{
    _ASSERT(m_oTarget!=0);

    OCQuatRotation *cCamOri = (OCQuatRotation*)getComponent("orientation");
    OCQuatRotation *cTarOri = (OCQuatRotation*)m_oTarget->getComponent("orientation");

    // Set the camera's orientation to be the same as the spaceship's
    cCamOri->setView  ( cTarOri->getView()  );
    cCamOri->setRight ( cTarOri->getRight() );
    cCamOri->setUp    ( cTarOri->getUp()    );
    
    setPos( m_oTarget->getPos() + 
           (cCamOri->getView()+cCamOri->getUp()) * m_vPosOffset );
}