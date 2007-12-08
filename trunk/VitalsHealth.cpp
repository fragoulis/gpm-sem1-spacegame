#include "VitalsHealth.h"
#include "Object.h"

namespace tlib 
{
    
    OCVitalsHealth::OCVitalsHealth( int iMaxHealth ):
    m_iMaxHealth(iMaxHealth),
    m_iHealth(iMaxHealth)
    {}

    void OCVitalsHealth::update()
    {
        if( isAlive() || hasEnded() ) return;
        
        // If health is below zero make the object inactive
        // [aka kill it]
        onKill();
        getOwner()->deactivate();
        m_bHasEnded = true;

        return;
    }

} // end of namespace tlib