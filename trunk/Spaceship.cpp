#include "Spaceship.h"
#include "VisualGXModel.h"
#include "AccelMovement.h"
#include "QuatRotation.h"
#include "SpaceshipKeyboard.h"
#include "SpaceshipCollisionResponse.h"
#include "Texture.h"
#include "CollisionGXModel.h"
#include "SpaceshipVitals.h"
#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "Logger.h"
#include "Config.h"

using namespace tlib;

Spaceship::Spaceship(): 
    m_bResetSpeed(0), 
    m_bResetRoll(0)
{}

void Spaceship::init()
{
    _LOG("Setting up spaceship...");

    // Set object's type
    setType( SPACESHIP );

    Config cfg("config.txt");
    cfg.loadBlock("spaceship");

    // Initialize position
    float vfPos[3];
    cfg.getFloat("init_pos", vfPos, 3);
    m_vPos.xyz( vfPos );

    // Initialize member variables
    cfg.getFloat("max_vel", &m_fMaxVelocity );
    cfg.getFloat("vel_factor", &m_fVelFactor );

    // ------------------------------
    m_fRoll = 0.0f;
    m_fMaxRoll = (float)M_PI_2;
    m_fRollFactor = 2.0f*(float)M_PI;
    m_fRotFactor = (float)M_PI_2;
    m_fRotBias = 3.0f;
    // ------------------------------

    // Initialize movement component
    setComponent( new OCAccelMovement );
 
    // Initialize visual component
    string sModel;
    float fScale;
    cfg.getString("model", sModel );
    cfg.getFloat("scale", &fScale );
    OCGXModel *model = new OCGXModel( sModel.c_str(), fScale );
    setComponent( model );
    
    // Correct the position of the spaceship by adding this offset
    m_vPos.sub( model->getGXModel().GetBoxCentre().x,
                model->getGXModel().GetBoxCentre().y,
                model->getGXModel().GetBoxCentre().z );

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

    // Initialize collision response component
    setComponent( new SpaceshipCollisionResponse );

    // Read laser color
    float vfLaserColor[4];
    cfg.getFloat("laser_color", vfLaserColor, 4);

    // Read laser emitter offset
    float fEmitterOffset;
    cfg.getFloat("laser_offset", &fEmitterOffset);

    // Read maximum lives and health
    int iMaxLives, iMaxHealth;
    cfg.getInt("lives", &iMaxLives);
    cfg.getInt("hits", &iMaxHealth);

    // Initialize health component
    setComponent( new SpaceshipVitals( iMaxLives, iMaxHealth ) );

    // Initialize laser system
    m_Laser = PSManager::Instance().addLaser( 
        this,
        fEmitterOffset, 
        vfLaserColor );

} // end setup()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::fire()
{
    if( !m_Laser->getEmitter().isOn() ) 
    {
        m_Laser->getEmitter().start();
    }
}

void Spaceship::ceaseFire() 
{
    m_Laser->getEmitter().stop();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::update()
{
    // Update keyboard calls for spaceship
    ((OCKeyboard*)getComponent("controller"))->update();

    // Update spaceship's position
    ((IOCMovement*)getComponent("movement"))->update();

    // Update spaceship's rotation
    //IOCOrientation *cShipOri = (IOCOrientation*)m_Ship.getComponent("orientation");
    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");
    cRot->update();

    // Update and check health status
    IOCVitals *cVitals = (IOCVitals*)getComponent("vitals");
    cVitals->update();
}

// ----------------------------------------------------------------------------
int Spaceship::getHealth()
{
    OCVitalsHealth *cVitals = (OCVitalsHealth*)getComponent("vitals");
    return int( 100 * cVitals->healthRatio() );
}

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
    //cLM->setDir( cRot->getView() );
    cLM->getDir().selfRotate( qPitch );

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
    //cLM->setDir( cRot->getView() );
    cLM->getDir().selfRotate( qPitch );

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
    //cLM->setDir( cRot->getView() );
    cLM->getDir().selfRotate( qYaw );
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
    //cLM->setDir( cRot->getView() );
    cLM->getDir().selfRotate( qYaw );
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
} // end resetRoll()