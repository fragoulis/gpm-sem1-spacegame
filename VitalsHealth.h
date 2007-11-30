#pragma once
#include "Vitals.h"

namespace tlib 
{
    class OCVitalsHealth : public IOCVitals
    {
    private:
        // The object's initial health
        int m_iMaxHealth;

        // The object's health
        int m_iHealth;

    public:
        /**
         * Constructor
         */
        OCVitalsHealth( int iMaxHealth );

        /**
         * Destructor
         */
        virtual ~OCVitalsHealth(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("vitalshealth"); 
        }

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
         * Checks the health status
         */
        bool isAlive() const {
            return (m_iHealth>0)?1:0;
        }

        /**
         * Applies damage to the object
         */
        void hit( int iAmount ) {
            if( m_iHealth > 0 )
                m_iHealth -= iAmount;
            else
                m_iHealth = 0;
        }

        /**
         * Heals the object
         */
        void heal( int iAmount ) {
            m_iHealth += iAmount;
            if( m_iHealth > m_iMaxHealth )
                m_iHealth = m_iMaxHealth;
        }

        /**
         * Gives object full health
         */
        void revive() {
            m_iHealth = m_iMaxHealth;
            onRevive();
        }

        /**
         * Returns the ration of the current health ot the 
         * maximum health
         */
        float healthRatio() {
            return (float)m_iHealth/m_iMaxHealth;
        }

    }; // end of OCVitalsHealth class

} // end of namespace tlib