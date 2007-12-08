
#pragma once
#include "Controller.h"
#include "Logger.h"
//#include "assert.h"
#include "GX/Joystick.h"
using gxbase::Joystick;

const int MAX_BUTTON = 10;

namespace tlib
{

    class OCGXJoystick : public IOCController
    {
    protected:
		// The joystick axis mapped to enumeration
		enum JoystickAxis {
			A_LEFT_LEFT,
			A_LEFT_RIGHT,
			A_LEFT_UP,
			A_LEFT_DOWN,

			A_RIGHT_LEFT,
			A_RIGHT_RIGHT,
			A_RIGHT_UP,
			A_RIGHT_DOWN
		};

		// Joystick's buttons mapped to enumeration
		enum JoystickButtons {
			B_LEFT = 0, B_TOP = 1, B_BOTTOM = 2, B_RIGHT = 3
		};

		// The gxbase joystick controller object
		Joystick m_Joystick;

		// Joystick's interface attributes
        float m_vfPosition[4];
		bool  m_vbButtons[MAX_BUTTON];

    public:
        /**
         * Constructor
         */
        OCGXJoystick()
        {
			m_vfPosition[0] = m_vfPosition[1] = 0.0f;
            memset( m_vbButtons, 0, sizeof(m_vbButtons) );
			if (!m_Joystick.Open())
				_LOG("This sample needs a joystick!");
        }

        /**
         * Destructor
         */
        virtual ~OCGXJoystick() {
			m_Joystick.Close();
		}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("gxjoystick"); 
        }

        //int getKey() const { return m_iKey; }
        //bool isDown() const { return m_bDown; }
        //int getKey( int k ) const 
        //{ 
        //    _ASSERT(k>=0&&k<256);
        //    return m_vbKeys[k]; 
        //}

    }; // end of OCKeyboard class

} // end of namespace tlib
