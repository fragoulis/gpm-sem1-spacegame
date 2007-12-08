#include "Spaceship.h"
#include "VisualGXModel.h"
#include "AccelMovement.h"
#include "QuatRotation.h"
//#include "Controller.h"
#include "SpaceshipKeyboard.h"
#include "SpaceshipJoystick.h"
#include "SpaceshipCollisionResponse.h"
#include "Texture.h"
#include "CollisionGXModel.h"
#include "SpaceshipVitals.h"
#include "ParticleSystemMgr.h"
#include "PSLaser.h"
#include "LightMgr.h"
#include "Clock.h"
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
    getPos().xyz( vfPos );

    // Initialize member variables
    cfg.getFloat("max_vel", &m_fMaxVelocity );
    cfg.getFloat("vel_factor", &m_fVelFactor );

    // ------------------------------
    m_fRoll = 0.0f;
    m_fMaxRoll = (float)M_PI_2;
    m_fRollFactor = (float)M_PI;
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
    getPos().sub( model->getGXModel().GetBoxCentre().x,
                  model->getGXModel().GetBoxCentre().y,
                  model->getGXModel().GetBoxCentre().z );

    // Initialize the rotation component
    OCQuatRotation *cRot = new OCQuatRotation;
    cRot->setView  ( Vector3f::Front() );
    cRot->setUp    ( Vector3f::Up() );
    cRot->setRight ( Vector3f::Right() );
    setComponent( cRot );

    // Initialize controll component
    setComponent( new SpaceshipKeyboard );
    //setComponent( new SpaceshipJoystick );

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

    // Read spotilight's cutoff
    float fCutOff, fExponent;
    cfg.getFloat("light_cutoff", &fCutOff);
    cfg.getFloat("light_exp", &fExponent);

    // Set light attributes
    // We are beign deterministic that light0 will always be of
    // the spotlight's
    m_Light.setId(0);
    m_Light.setCutOff( fCutOff );
    m_Light.setExponent( fExponent );
    
} // end setup()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::applyLight()
{
    OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");

    // Update light's position and direction
    m_Light.setPos( getPos() );
    m_Light.setDir( cRot->getView() );
}

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
    ((IOCController*)getComponent("controller"))->update();

    // Update spaceship's position
    ((IOCMovement*)getComponent("movement"))->update();

    // Update spaceship's rotation
	((OCQuatRotation*)getComponent("orientation"))->update();

    // Update and check health status
	((IOCVitals*)getComponent("vitals"))->update();
}

// ----------------------------------------------------------------------------
void Spaceship::getVitals( int &iHealthRatio, int &iLives, int &iMaxLives )
{
    OCVitalsLives *cVitals = (OCVitalsLives*)getComponent("vitals");
    iHealthRatio = int( 100 * cVitals->healthRatio() );
    iLives = cVitals->getLives();
    iMaxLives = cVitals->getMaxLives();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::speed( float fAxis )
{
	OCAccelMovement *cMov = (OCAccelMovement*)getComponent("movement");
    if( cMov->getVelocity() > m_fMaxVelocity ) {
		//cMov->setVelocity( m_fMaxVelocity );
        cMov->setAccel( 0.0f );
    } else {
        cMov->setAccel( m_fVelFactor * fAxis );
    }
	m_bResetSpeed = true;
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
	if( cMov->getVelocity() > 0.1f )
	{
		//std::cout << "RESET SPEED SLOW DOWN" << std::endl;
		cMov->setAccel( -m_fVelFactor );
	}
	else if( cMov->getVelocity() < -0.1f ) 
	{
		//std::cout << "RESET SPEED SPEED UP" << std::endl;
		cMov->setAccel( m_fVelFactor );
	}
	else {
		//std::cout << "NO SPEED" << std::endl;
		cMov->setVelocity( 0.0f );
        cMov->setAccel( 0.0f );
        m_bResetSpeed = false;
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::move( float fVert, float fHoriz )
{
	//std::cout << fVert << " ___ " << fHoriz << std::endl;
	OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");
	OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
	float fVelRatio = cLM->getVelocity() / m_fMaxVelocity;

	if( fabs(fVert) < 1e-3f ) {
		if( fHoriz > 1e-3f ) fVert = -fVelRatio;
		else if( fHoriz < -1e-3f ) fVert = fVelRatio;
	}
	
	// Add roll only if the maximum roll has not been reached
	if( m_fRoll < -m_fMaxRoll || m_fRoll > m_fMaxRoll )
	{
		// Calculate the roll factor
		const float fRollFactor = m_fRollFactor * fHoriz * fVelRatio * 
							      Clock::Instance().getDeltaTime();
		m_fRoll += fRollFactor;
		// Add roll
		cRot->addRoll( fRollFactor );
	}

	// Calculate the pitch factor
	const float fPitchFactor = m_fRotFactor * -fVert * 
							   Clock::Instance().getDeltaTime();
	// Add pitch
	cRot->addPitch( fPitchFactor );

	// Update ship's orientation vectors
	cRot->updateView( cRot->getRot() );
	cRot->updateUp( cRot->getRot() );
	cRot->updateRight();

    // Update view vector
    cLM->setDir( cRot->getView() );
    //cLM->getDir().selfRotate( qTotalRot );
} // end move()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::resetMove()
{
	OCQuatRotation *cRot = (OCQuatRotation*)getComponent("orientation");
	
	if( fabs(m_fRoll) > 0.1f )
	{
		float fRollFactor;
		if( m_fRoll > 0.1f ) {
			// Calculate the roll factor
			fRollFactor = -m_fRollFactor * Clock::Instance().getDeltaTime();
		}
		else if( m_fRoll < -0.1 ) {
			// Calculate the roll factor
			fRollFactor = m_fRollFactor * Clock::Instance().getDeltaTime();
		}

		m_fRoll += fRollFactor;
		// Add roll
		cRot->addRoll( fRollFactor );

		// Update ship's orientation vectors
		cRot->updateUp( cRot->getRot() );
		cRot->updateRight();
	} // end if( ... )
} // end resetMove()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Spaceship::moveUp()
{
    OCLinearMovement *cLM = (OCLinearMovement*)getComponent("movement");
    float
        // Calculate 'half a' velocity ratio
        fVelRatio = cLM->getVelocity() / ( m_fRotBias * m_fMaxVelocity ),
        // Calculate the pitch factor
        fPitchFactor = m_fRotFactor * Clock::Instance().getDeltaTime() * ( 1.0f - fVelRatio );

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
        fPitchFactor = m_fRotFactor * Clock::Instance().getDeltaTime() * ( 1.0f - fVelRatio );

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
        fYawFactor = m_fRotFactor * Clock::Instance().getDeltaTime() * ( 1.0f - fVelRatio ),
        fRollFactor = m_fRollFactor * Clock::Instance().getDeltaTime() * fVelRatio;

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
        fYawFactor = m_fRotFactor * Clock::Instance().getDeltaTime() * ( 1.0f - fVelRatio ),
        fRollFactor = m_fRollFactor * Clock::Instance().getDeltaTime() * fVelRatio;

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
    float fRollFactor = m_fRollFactor * Clock::Instance().getDeltaTime();

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