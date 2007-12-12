#pragma once
#include "Controller.h"
#include "Logger.h"
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
			A_RIGHT_UP,
			A_RIGHT_DOWN,
			NUM_AXIS
		};

		// Joystick's buttons mapped to enumeration
		enum JoystickButtons {
			B_LEFT = 0, B_TOP = 1, B_BOTTOM = 2, B_RIGHT = 3
		};

		// The gxbase joystick controller object
		Joystick m_Joystick;

		// Joystick's interface attributes
        float m_vfPosition[NUM_AXIS];
		bool  m_vbButtons[MAX_BUTTON];

    public:
        /**
         * Constructor
         */
        OCGXJoystick();

        /**
         * Destructor
         */
        virtual ~OCGXJoystick();

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("gxjoystick"); 
        }

    }; // end of OCGXJoystick class

} // end of namespace tlib
