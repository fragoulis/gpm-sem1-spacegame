#pragma once
#include "TimerAnimation.h"
#include "Color.h"
using tlib::OCTimerAnimation;

class OutletAnimation : public OCTimerAnimation
{
private:
    // The outlet's blinking color
    Color m_BlinkColor;

public:
    /**
     * Constructor
     */
    OutletAnimation();

    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("outletanimation"); 
    }

private:
    // empty
    void onStart();
    /**
     * Resets the color to the initial white
     */
    void onStop();

    /**
     * Makes the power outlet switch between colors
     */
    void OnTimerChange( unsigned int uiCurrentTime,
                        unsigned int uiDuration ) const;

}; // end of OutletAnimation class