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
        if( isAlive() ) return;
        
        // If health is below zero make the object inactive
        // [aka kill it]
        onKill();
        getOwner()->deactivate();
    }

} // end of namespace tlib