#pragma once
#include "Animation.h"
#include "Movement.h"
using tlib::IOCAnimation;
using tlib::IOCMovement;

class DoorAnimation : public IOCAnimation
{
public:
    /**
     * Returns the unique component ID
     */
    const string componentID() const { 
        return string("dooranimation"); 
    }

private:
    void onStart() {}
    void onStop() {}
    void onUpdate() {
        IOCMovement *cMov = (IOCMovement*)getOwner()->getComponent("movement");
        cMov->update();
    }
    bool condition() const { 
        return 1; 
    }

}; // end of DoorAnimation class