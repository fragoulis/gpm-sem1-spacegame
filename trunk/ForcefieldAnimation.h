#pragma once
#include "Animation.h"
using tlib::IOCAnimation;

class ForcefieldAnimation : public IOCAnimation
{
public:
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("forcefieldanimation"); 
    }

private:
    /**
     * Disable the collision box
     */
    void onStart();
    // empty
    void onStop();

    /**
     * Update the door's panels
     */
    void onUpdate();

    /**
     * Check for reason to stop animation
     */
    bool condition();

}; // end of ForcefieldAnimation class