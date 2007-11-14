#pragma once
#include "Barrier.h"
#include "DoorPanel.h"

class Door : public Barrier
{
public:
    // The number of panels
    static const int ciNumOfPanels = 4;

    // A single panel's half dimensions
    // We declare it static because it will be the same for
    // all doors and panels
    static Vector3f PanelDim;

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
     * Four methods each returning a different panel
     */
    DoorPanel* leftPanel() {
        return getPanel( DoorPanel::Left );
    }
    DoorPanel* rightPanel() {
        return getPanel( DoorPanel::Right );
    }
    DoorPanel* topPanel() {
        return getPanel( DoorPanel::Top );
    }
    DoorPanel* bottomPanel() {
        return getPanel( DoorPanel::Bottom );
    }

private:
    /**
     * Returns a panel by its type
     */
    DoorPanel* getPanel( DoorPanel::PanelId pid ) {
        return &m_Panel[pid];
    }
};