#pragma once
#include "Animation.h"
using tlib::IOCAnimation;

class BladeDeathAnimation : public IOCAnimation
{
public:
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("bladedeathanimation"); 
    }

private:
    // empty
    void onStart();
    // empty
    void onStop();

    /**
     * Update the rotating blade
     */
    void onUpdate();

    // empty
    bool condition();

}; // end of BladeDeathAnimation class