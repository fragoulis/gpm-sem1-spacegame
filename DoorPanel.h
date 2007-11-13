#pragma once
#include "Object.h"
using tlib::Object;

class DoorPanel : public Object
{
public:
    /**
     * An enumeration of what a door panel can be.
     * Actually a door has four panels, and this is 
     * what we need to know for where a panel sits
     */
    enum PanelId {
        Left = 0,
        Right,
        Top,
        Bottom
    };

    /** 
     * Initializes the panel
     */
    void init( PanelId pid );
};