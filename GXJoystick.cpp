#include "GXJoystick.h"

namespace tlib
{

	OCGXJoystick::OCGXJoystick()
    {
		m_vfPosition[0] = m_vfPosition[1] = 0.0f;
        memset( m_vbButtons, 0, sizeof(m_vbButtons) );
        if (!m_Joystick.Open()) {
			_LOG("This sample needs a joystick!");
        }
    }

	OCGXJoystick::~OCGXJoystick() 
	{
		m_Joystick.Close();
	}

} // end of namespace tlib
