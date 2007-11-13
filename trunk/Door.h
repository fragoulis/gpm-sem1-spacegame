#pragma once
#include "Barrier.h"
#include "DoorPanel.h"

class Door : public Barrier
{
public:
    // The number of panels
    static const int ciNumOfPanels = 4;

private:
    // The four door panels
    // Here we 
    DoorPanel m_Panel[ciNumOfPanels];

public:
    /**
     * Constructor
     */
    Door();

    /**
     * Returns a panel by its type
     */
    const DoorPanel& getPanel( DoorPanel::PanelId pid ) {
        return m_Panel[pid];
    }
};