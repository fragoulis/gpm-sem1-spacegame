#include "VitalsHealth.h"
#include "Object.h"

namespace tlib 
{
    
    OCVitalsHealth::OCVitalsHealth( int iMaxHealth ):
    m_iMaxHealth(iMaxHealth),
    m_iHealth(iMaxHealth)
    {}

    bool OCVitalsHealth::update()
    {
        if( isAlive() ) return true;
        
        // If health is below zero make the object inactive
        // [aka kill it]
        onKill();
        getOwner()->deactivate();

        return false;
    }

} // end of namespace tlib