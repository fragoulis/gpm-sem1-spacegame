#include "SpaceshipJoystick.h"
#include "Spaceship.h"
#include "Recorder.h"
#include "CameraMgr.h"
#include "ObjectMgr.h"
#include "Autopilot.h"

const float AXIS_ZERO_P = 3.05176e-005f;
const float AXIS_ZERO_N = -3.05176e-005f;

// Control variables
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
        // Check if autopilot is on, otherwise the ship is just dead
        if( Autopilot::Instance().isActive() )
        {
            // Stop replay if we are in replay mode
            if( Recorder::Instance().isReplayOn() )
                Recorder::Instance().endReplay();
            else
                // Stop recording if we are recording
                if( Recorder::Instance().isRecordOn() )
                    Recorder::Instance().stop();
            
            // We could force a third-person view
            //CameraMgr::Instance().activate("stalker");
            ////ObjectMgr::Instance().showShip();
            //CameraMgr::Instance().activate("first-person");
            //ObjectMgr::Instance().hideShip();
        } 
        else 
        {
            // Ship is dead
            ship->resetSpeed();
        }
    
        ship->ceaseFire();
        return;
    }

    if( Recorder::Instance().isReplayOn() )
    {
        // Read joystick inputs from file
		if( !Recorder::Instance().isOpen() ) {
			return;
		}

		Recorder::Instance().replayAxis( m_vfPosition, NUM_AXIS );
        Recorder::Instance().replayButton( m_vbButtons, MAX_BUTTON );
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
        if( Recorder::Instance().isRecordOn() )
        {
            // Record axis movement
			Recorder::Instance().recordAxis( m_vfPosition, NUM_AXIS );
            // Record button presses
            Recorder::Instance().recordButton( m_vbButtons, MAX_BUTTON );

            Recorder::Instance().flush();
        }
    } // end if( g_bIsReplay ) ... else 

    // Control speed
	if( m_vfPosition[A_RIGHT_UP] > AXIS_ZERO_P ) {
		ship->speed( m_vfPosition[A_RIGHT_UP] );
	}
	else if( m_vfPosition[A_RIGHT_DOWN] > AXIS_ZERO_P ) {
		ship->speed( -m_vfPosition[A_RIGHT_DOWN] );
		
	} else {
		ship->resetSpeed();
	}
	
    // Control rotations
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

	if( m_vbButtons[B_TOP] && g_iSingleInterval<0 && 
        !Recorder::Instance().isReplayOn() ) 
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

} // end update()