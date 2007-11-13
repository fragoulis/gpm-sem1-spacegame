#include "Spaceship.h"
#include "Logger.h"
#include "TPCamera.h"
#include "Config.h"
#include "VisualGXModel.h"
#include "AccelMovement.h"
#include "QuatRotation.h"
#include "SpaceshipKeyboard.h"
#include "Texture.h"
#include "CollisionGXModel.h"

using namespace tlib;

Spaceship::Spaceship(): 
    m_bResetSpeed(0), 
    m_bResetRoll(0),
    m_bInStation(0), 
    m_bInReactor(0),
    m_TPCamera(0),
    m_CurrentTile(0)
{}

void Spaceship::setup()
{
    _LOG("Setting up spaceship...");

    Config cfg("config.txt");
    cfg.loadBlock("spaceship");

    // Initialize position
    float vfPos[3];
    cfg.getFloat("init_pos", vfPos, 3);
    m_vPos.xyz( vfPos );

    // Initialize member variables
    cfg.getFloat("max_vel", &m_fMaxVelocity );
    cfg.getFloat("vel_factor", &m_fVelFactor );

    m_fRoll = 0.0f;
    m_fMaxRoll = (float)M_PI_2;
    m_fRollFactor = 2.0f*(float)M_PI;

    m_fRotFactor = (float)M_PI_2;

    m_fRotBias = 2.0f;

    // Initialize movement component
    setComponent( new OCAccelMovement );
 
    // Initialize visual component
    string sModel;
    float fScale;
    cfg.getString("model", sModel );
    cfg.getFloat("scale", &fScale );
    setComponent( new OCGXModel( sModel.c_str(), fScale ) );

    // Initialize the rotation component
    OCQuatRotation *cRot = new OCQuatRotation;
    cRot->setView  ( Vector3f::Front() );
    cRot->setUp    ( Vector3f::Up() );
    cRot->setRight ( Vector3f::Right() );
    setComponent( cRot );

    // Initialize controll component
    setComponent( new OCSpaceshipKeyboard );
    //setComponent( new OCSpaceshipJoystick );

    // Initialize collision component
    // [Here we give our object its bounding box]
    OCCollisionGXModel *cCol = new OCCollisionGXModel;
    setComponent( cCol );
    cCol->initBBox();

} // end setup()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::speedUp()
{
    OCAccelMovement *cMov = (OCAccelMovement*)getComponent("movement");
    if( cMov->getVelocity() > m_fMaxVelocity ) {
        cMov->setAccel( 0.0f );
    } else {
        cMov->setAccel( m_fVelFactor );
    }
    m_bResetSpeed = true;
}

// ----------------------------------------------------------------------------
void Spaceship::speedDown()
{
    OCAccelMovement *cMov = (OCAccelMovement*)getComponent("movement");
    if( cMov->getVelocity() < -m_fMaxVelocity ) {
        cMov->setAccel( 0.0f );
    } else {
        cMov->setAccel( -m_fVelFactor );
    }
    m_bResetSpeed = true;
}

// ----------------------------------------------------------------------------
void Spaceship::resetSpeed()
{
    if( !m_bResetSpeed ) return;

	OCAccelMovement *cMov = (OCAccelMovement*)getComponent("movement");
	if( cMov->getVelocity() > 1e-6 )
	{
		cMov->setAccel( -m_fVelFactor );
	}
	else if( cMov->getVelocity() < -1e-6 ) 
	{
		cMov->setAccel( m_fVelFactor );
	}
	else {
		cMov->setVelocity( 0.0f );
        cMov->setAccel( 0.0f );
        m_bResetSpeed = false;
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::moveUp()
{
    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float
        // Calculate 'half a' velocity ratio
        fVelRatio = cLM->getVelocity() / ( m_fRotBias * m_fMaxVelocity ),
        // Calculate the pitch factor
        fPitchFactor = m_fRotFactor * IOCMovement::DeltaTime() * ( 1.0f - fVelRatio );

    // Rotate ship about right axis
    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");
    Quatf qPitch = cRot->addPitch( fPitchFactor );

    cRot->updateView( qPitch );
    cRot->updateUp( qPitch );

    // Update view vector
    cLM->setDir( cRot->getView() );

    // Tell the camera to follow
    m_TPCamera->activate();
}

// ----------------------------------------------------------------------------
void Spaceship::moveDown()
{
    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float
        // Calculate 'half a' velocity ratio
        fVelRatio = cLM->getVelocity() / ( m_fRotBias * m_fMaxVelocity ),
        // Calculate the pitch factor
        fPitchFactor = m_fRotFactor * IOCMovement::DeltaTime() * ( 1.0f - fVelRatio );

    // Rotate ship about right axis
    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");
    Quatf qPitch = cRot->addPitch( -fPitchFactor );

    cRot->updateView( qPitch );
    cRot->updateUp( qPitch );

    // Update direction vector
    cLM->setDir( cRot->getView() );

    // Tell the camera to follow
    m_TPCamera->activate();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::moveLeft()
{
    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float
        fVelRatio = cLM->getVelocity() / ( m_fRotBias * m_fMaxVelocity ),
        fYawFactor = m_fRotFactor * IOCMovement::DeltaTime() * ( 1.0f - fVelRatio ),
        fRollFactor = m_fRollFactor * IOCMovement::DeltaTime() * fVelRatio;

    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");

    // Rotate ship about up axis
    Quatf qYaw = cRot->addYaw( fYawFactor );
    
    cRot->updateView( qYaw );
    cRot->updateRight();

    // Roll
    if( m_fRoll < m_fMaxRoll )
    {
        // Rotate ship about view axis
        //cRot->addRoll( -fRollFactor );
        // Update total roll
        //m_fRoll += fRollFactor;

        m_bResetRoll = true;
    }

    // Update direction vector
    cLM->setDir( cRot->getView() );

    // Tell the camera to follow
    m_TPCamera->activate();
}

// ----------------------------------------------------------------------------
void Spaceship::moveRight()
{
    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float
        fVelRatio = cLM->getVelocity() / ( m_fRotBias * m_fMaxVelocity ),
        fYawFactor = m_fRotFactor * IOCMovement::DeltaTime() * ( 1.0f - fVelRatio ),
        fRollFactor = m_fRollFactor * IOCMovement::DeltaTime() * fVelRatio;

    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");

    // Rotate ship about up axis
    Quatf qYaw = cRot->addYaw( -fYawFactor );

    cRot->updateView( qYaw );
    cRot->updateRight();

    // Roll
    if( m_fRoll > -m_fMaxRoll )
    {
        // Rotate ship about view axis
        //cRot->addRoll( fRollFactor );
        // Update total roll
        //m_fRoll -= fRollFactor;

        m_bResetRoll = true;
    }
        
    // Update direction vector
    cLM->setDir( cRot->getView() );

    // Tell the camera to follow
    m_TPCamera->activate();
}

// ----------------------------------------------------------------------------
void Spaceship::resetRoll()
{
    if( !m_bResetRoll ) return;

    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float fRollFactor = m_fRollFactor * IOCMovement::DeltaTime();

    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");    

    // Roll
    //std::cout << m_fRoll << std::endl;

    if( m_fRoll < -1e-2f )
    {
        // Rotate ship about view axis
        cRot->addRoll( -fRollFactor );
        // Update total roll
        m_fRoll += fRollFactor;
        //std::cout << "ADDED TO IT" << std::endl;
    }
    else if( m_fRoll > 1e-2f )
    {
        // Rotate ship about view axis
        cRot->addRoll( fRollFactor );
        // Update total roll
        m_fRoll -= fRollFactor;
        //std::cout << "SUB FROM IT" << std::endl;
    } else {
        //std::cout << "MADE IT ZERO" << std::endl;
        m_fRoll = 0.0f;
        m_bResetRoll = false;
    }
}