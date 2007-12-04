#pragma once
#include "TimerAnimation.h"
#include "Color.h"
using tlib::OCTimerAnimation;

class OutletAnimation : public OCTimerAnimation
{
private:
    // The outlet's colors
    // Color for when the outlet is off
    Color m_colOff;
    
    // Color for when the outlet blinks
    Color m_colBlink;

    // Color for when the outlet is on[aka done]
    Color m_colOn;

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