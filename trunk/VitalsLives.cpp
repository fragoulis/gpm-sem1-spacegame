#include "VitalsLives.h"
#include "Object.h"

namespace tlib 
{
    
    OCVitalsLives::OCVitalsLives( int iMaxLives, int iMaxHealth ):
    OCVitalsHealth( iMaxHealth ),
    m_iMaxLives(iMaxLives),
    m_iLives(iMaxLives)
    {}

    void OCVitalsLives::update()
    {
        if( isAlive() || hasEnded() ) return;
        
        // If health is below zero make the object inactive
        // [aka kill it]
        kill();

        // If object has remaining lives revive it
        if( m_iLives > 0 ) {
            revive();
        }
        else {
            // Else deactivate it
            getOwner()->deactivate();
            m_bHasEnded = true;
            onEndOfLives();
        }
    }

} // end of namespace tlib