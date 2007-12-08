#include "SpaceshipJoystick.h"
#include "Spaceship.h"

const float AXIS_ZERO_P = 3.05176e-005f;
const float AXIS_ZERO_N = -3.05176e-005f;

void SpaceshipJoystick::update()
{
    Spaceship *ship = (Spaceship*)getOwner();
	m_Joystick.Update();

	// Map  joystick controls to our spacecraft
	m_vfPosition[A_LEFT_RIGHT]	= m_Joystick.Axis(0);
	m_vfPosition[A_LEFT_LEFT]	= -m_Joystick.Axis(0);
	m_vfPosition[A_LEFT_DOWN]	= m_Joystick.Axis(1);
	m_vfPosition[A_LEFT_UP]		= -m_Joystick.Axis(1);

	m_vfPosition[A_RIGHT_DOWN]	= m_Joystick.Axis(2);
	m_vfPosition[A_RIGHT_UP]	= -m_Joystick.Axis(2);
	//m_vfPosition[A_RIGHT_RIGHT]	= m_Joystick.Axis(3);
	//m_vfPosition[A_RIGHT_LEFT]	= -m_Joystick.Axis(3);

	for( int n=0; n<MAX_BUTTON; n++ )
		m_vbButtons[n] = m_Joystick.Button(n);

	//if( m_vfPosition[A_LEFT_RIGHT] )
	//	std::cout << "LEFT AXIS -> RIGHT " << m_vfPosition[A_LEFT_RIGHT] << std::endl;
	//if( m_vfPosition[A_LEFT_LEFT] )
	//	std::cout << "LEFT AXIS -> LEFT " << m_vfPosition[A_LEFT_LEFT] << std::endl;
	//if( m_vfPosition[A_LEFT_DOWN] )
	//	std::cout << "LEFT AXIS -> DOWN " << m_vfPosition[A_LEFT_DOWN] << std::endl;
	//if( m_vfPosition[A_LEFT_UP] )
	//	std::cout << "LEFT AXIS -> UP " << m_vfPosition[A_LEFT_UP] << std::endl;

	//if( m_vfPosition[A_RIGHT_RIGHT] )
	//	std::cout << "RIGHT AXIS -> RIGHT " << m_vfPosition[A_LEFT_RIGHT] << std::endl;
	//if( m_vfPosition[A_RIGHT_LEFT] )
	//	std::cout << "RIGHT AXIS -> LEFT " << m_vfPosition[A_RIGHT_LEFT] << std::endl;
	//if( m_vfPosition[A_RIGHT_DOWN] )
	//	std::cout << "RIGHT AXIS -> DOWN " << m_vfPosition[A_RIGHT_DOWN] << std::endl;
	//if( m_vfPosition[A_RIGHT_UP] )
	//	std::cout << "RIGHT AXIS -> UP " << m_vfPosition[A_RIGHT_UP] << std::endl;

	//if( m_vbButtons[B_LEFT] )
	//	std::cout << "BUTTON B_LEFT" << std::endl;
	//if( m_vbButtons[B_TOP] )
	//	std::cout << "BUTTON B_TOP" << std::endl;
	//if( m_vbButtons[B_BOTTOM] )
	//	std::cout << "BUTTON B_BOTTOM" << std::endl;
	//if( m_vbButtons[B_RIGHT] )
	//	std::cout << "BUTTON B_RIGHT" << std::endl;

	if( m_vfPosition[A_RIGHT_UP] > AXIS_ZERO_P ) {
		ship->speed( m_vfPosition[A_RIGHT_UP] );
	}
	else if( m_vfPosition[A_RIGHT_DOWN] > AXIS_ZERO_P ) {
		ship->speed( -m_vfPosition[A_RIGHT_DOWN] );
		
	} else {
		ship->resetSpeed();
	}
	
	if( ( m_vfPosition[A_LEFT_UP]   > AXIS_ZERO_P ||
		  m_vfPosition[A_LEFT_DOWN] > AXIS_ZERO_P ) &&
		( m_vfPosition[A_LEFT_RIGHT]> AXIS_ZERO_P ||
		  m_vfPosition[A_LEFT_LEFT] > AXIS_ZERO_P ) ) 
	{
		ship->move( m_vfPosition[A_LEFT_UP], m_vfPosition[A_LEFT_RIGHT] );
	} 
	else if( m_vfPosition[A_LEFT_UP]   > AXIS_ZERO_P ||
		     m_vfPosition[A_LEFT_DOWN] > AXIS_ZERO_P ) 
	{
		ship->move( m_vfPosition[A_LEFT_UP], 0.0f );
	} 
	else if( m_vfPosition[A_LEFT_RIGHT]> AXIS_ZERO_P ||
		     m_vfPosition[A_LEFT_LEFT] > AXIS_ZERO_P ) 
	{
		ship->move( 0.0f, m_vfPosition[A_LEFT_RIGHT] );
	} else {
		//ship->resetMove();
	}

	//if( m_vfPosition[A_RIGHT_UP] > AXIS_ZERO_P ) {
	//	ship->speedUp();
	//}
	//else if( m_vfPosition[A_RIGHT_DOWN] > AXIS_ZERO_P ) {
	//	ship->speedDown();	
	//}
	//else {
	//	ship->resetSpeed();
	//}

    //if( getKey('f') ) {
    //    ship->moveUp();
    //}
    //else if( getKey('r') ) {
    //    ship->moveDown();
    //}

    //if( getKey('g') ) {
    //    ship->moveRight();
    //}
    //else if( getKey('d') ) {
    //    ship->moveLeft();
    //} else {
    //    ship->resetRoll();
    //}

    if( m_vbButtons[B_BOTTOM] ) ship->fire();
    else ship->ceaseFire();

    //else if( getKey('t') ) getOwner()->toggleCam();
    //else if( getKey('r') ) getOwner()->record();
}