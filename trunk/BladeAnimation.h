#pragma once
#include "Animation.h"
using tlib::IOCAnimation;

class BladeAnimation : public IOCAnimation
{
private:
    // Helper variables
    float m_fDistMoved, m_fPrevPos;

public:
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("bladeanimation"); 
    }

private:
    /**
     * Disable the collision box
     */
    void onStart();
    // empty
    void onStop();

    /**
     * Update the rotating blade
     */
    void onUpdate();

    /**
     * Check for reason to stop animation
     */
    bool condition();

}; // end of BladeAnimation class