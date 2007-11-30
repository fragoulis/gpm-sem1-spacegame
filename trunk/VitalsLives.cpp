#include "VitalsLives.h"
#include "Object.h"

namespace tlib 
{
    
    OCVitalsLives::OCVitalsLives( int iMaxLives, int iMaxHealth ):
    OCVitalsHealth( iMaxHealth ),
    m_iMaxLives(iMaxLives),
    m_iLives(iMaxLives)
    {}

    bool OCVitalsLives::update()
    {
        if( isAlive() ) return true;
        
        // If health is below zero make the object inactive
        // [aka kill it]
        kill();

        // If object has remaining lives revive it
        if( m_iLives > 0 ) {
            revive();
            return true;
        }
        else 
            // Else deactivate it
            getOwner()->deactivate();

        return false;
    }

} // end of namespace tlib