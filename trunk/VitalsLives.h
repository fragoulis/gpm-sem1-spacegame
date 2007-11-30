#pragma once
#include "VitalsHealth.h"

namespace tlib 
{
    class OCVitalsLives : public OCVitalsHealth
    {
    protected:
        // The object's maximum number of lives
        int m_iMaxLives;

        // The object's number of lives
        int m_iLives;

    public:
        /**
         * Constructor
         */
        OCVitalsLives( int iMaxLives, int iMaxHealth );

        /**
         * Destructor
         */
        virtual ~OCVitalsLives(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("vitalslives"); 
        }

        /**
         * Returns the number of remaining lives
         */
        int getLives() const { return m_iLives; }

        /**
         * Checks the health status and takes care of the object's
         * death
         */
        virtual bool update();

        /**
         * This is called on an object's death
         */
        virtual void onKill() = 0;

        /**
         * This is called on an object's revival
         */
        virtual void onRevive() = 0;

        /**
         * Reduces the lives of the object and call onKill callback
         */
        void kill() {
            --m_iLives;
            onKill();
        }

    }; // end of OCVitalsLives class

} // end of namespace tlib