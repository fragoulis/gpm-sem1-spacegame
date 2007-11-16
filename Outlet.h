#pragma once
#include "TileObject.h"

class Barrier;

class Outlet : public TileObject 
{
private:
    // Pointer to the barrier with which this outlet is
    // associated
    Barrier *m_pBarrier;

public:
    /**
     * Constructor
     */
    Outlet();

    /**
     * Getter/Setter for the barrier pointer
     */
    Barrier* getBarrier() const { return m_pBarrier; }
    void setBarrier( Barrier *bar ) { m_pBarrier = bar; }
};