#include "SpaceshipJoystick.h"
#include "Spaceship.h"
#include "Recorder.h"
#include "CameraMgr.h"
#include "ObjectMgr.h"

const float AXIS_ZERO_P = 3.05176e-005f;
const float AXIS_ZERO_N = -3.05176e-005f;

// Control variables
extern bool g_bIsReplay;
extern bool g_bSwitchCam;
extern bool g_bToggleRecord;

// Static counter to control button presses
static const int g_ciInterval = 40;
static int g_iSingleInterval = 0;

void SpaceshipJoystick::update()
{
    Spaceship *ship = (Spaceship*)getOwner();
    if( !ship->isActive() ) 
    {
        ship->resetSpeed();
        ship->ceaseFire();
        return;
    }

    if( g_bIsReplay )
    {
        // Read joystick inputs from file
		if( !Recorder::Instance().isOpen() ) {
			return;
		}

		Recorder::Instance().replayAxis( m_vfPosition, NUM_AXIS );
        Recorder::Instance().replayButton( m_vbButtons, MAX_BUTTON );
		//Recorder::Instance().replayEnd();
    } 
    else 
    {
	    m_Joystick.Update();

	    // Map  joystick controls to our spacecraft
	    m_vfPosition[A_LEFT_RIGHT]	=  m_Joystick.Axis(0);
	    m_vfPosition[A_LEFT_LEFT]	= -m_Joystick.Axis(0);
	    m_vfPosition[A_LEFT_DOWN]	=  m_Joystick.Axis(1);
	    m_vfPosition[A_LEFT_UP]		= -m_Joystick.Axis(1);

	    m_vfPosition[A_RIGHT_UP]	= -m_Joystick.Axis(2);
		m_vfPosition[A_RIGHT_DOWN]	=  m_Joystick.Axis(2);

		for( int n=0; n<MAX_BUTTON; ++n ) {
		    m_vbButtons[n] = m_Joystick.Button(n);
		}

        // Record user inputs
        if( Recorder::Instance().isOn() )
        {
            // Record axis movement
			Recorder::Instance().recordAxis( m_vfPosition, NUM_AXIS );
            // Record button presses
            Recorder::Instance().recordButton( m_vbButtons, MAX_BUTTON );

            Recorder::Instance().flush();
        }
    } // end if( g_bIsReplay ) ...

	if( m_vfPosition[A_RIGHT_UP] > AXIS_ZERO_P ) {
		ship->speed( m_vfPosition[A_RIGHT_UP] );
	}
	else if( m_vfPosition[A_RIGHT_DOWN] > AXIS_ZERO_P ) {
		ship->speed( -m_vfPosition[A_RIGHT_DOWN] );
		
	} else {
		ship->resetSpeed();
	}
	
	if( ( m_vfPosition[A_LEFT_UP]   > AXIS_ZERO_P ||
		  m_vfPosition[A_LEFT_DOWN] > AXIS_ZERO_P ) ||
		( m_vfPosition[A_LEFT_RIGHT]> AXIS_ZERO_P ||
		  m_vfPosition[A_LEFT_LEFT] > AXIS_ZERO_P ) ) 
	{
		ship->move( m_vfPosition[A_LEFT_UP], m_vfPosition[A_LEFT_RIGHT] );
	} 

	// Left button switch [Record mode]
    if( m_vbButtons[B_BOTTOM] ) ship->fire();
    else ship->ceaseFire();

	if( m_vbButtons[B_TOP] && g_iSingleInterval<0 && !g_bIsReplay ) 
	{
		if( g_bToggleRecord ) {
            Recorder::Instance().stop();
        } else {
            if( !Recorder::Instance().start() ) {
                _LOG("Failed to open output file for recording");
            }
        }
		g_bToggleRecord = !g_bToggleRecord;
		g_iSingleInterval = g_ciInterval;
	}
	
	// Right button switch [Camera toggle]
	if( m_vbButtons[B_RIGHT] && g_iSingleInterval<0 ) 
	{
		if( g_bSwitchCam ) 
		{
            CameraMgr::Instance().activate("first-person");
            ObjectMgr::Instance().hideShip();
        } else {
            CameraMgr::Instance().activate("stalker");
            ObjectMgr::Instance().showShip();
        }
		g_bSwitchCam = !g_bSwitchCam;
		g_iSingleInterval = g_ciInterval;
	}
	--g_iSingleInterval;

}