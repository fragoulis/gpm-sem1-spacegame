#pragma once
#include "Animation.h"

namespace tlib
{

    class OCTimerAnimation : public IOCAnimation
    {
    private:
        // How long the animation will last in miliseconds
        unsigned long m_ulDuration;

        // The current time
        unsigned long m_ulCurrent;

    public:
        /**
         * Constructor
         */
        OCTimerAnimation(): m_ulDuration(0L), m_ulCurrent(0L)
        {}
        OCTimerAnimation( bool bRepeat ): m_ulDuration(0L), m_ulCurrent(0L)
        {
            IOCAnimation::IOCAnimation( bRepeat );
        }

        /**
         * Destructor
         */
        virtual ~OCTimerAnimation(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("timeranimation"); 
        }

        /**
         * Getter/Setter for the animation's duration
         */
        unsigned long getDuration() const { return m_ulDuration; }
        void setDuration( unsigned long ulDuration ) { 
            m_ulDuration = ulDuration; 
        }

    protected:
        /**
         * Override animation controls here
         * The timer animation component only overrides the termination
         * conditions of the animation and the update callback. In place
         * of the update, gives a more usefull one to the client, with
         * the current time as a parameter
         */
        virtual void onStart() = 0;

        virtual void onStop() = 0;

        /**
         * This function is to be called by the timer every time
         * it advances
         */
        virtual void OnTimerChange( unsigned long ulCurrentTime ) const = 0;

    private:
        void onUpdate() 
        { 
            m_ulCurrent++; 
            OnTimeChange( m_ulCurrent );
        }

        bool condition() const
        {
            if( m_ulCurrent >= m_ulDuration ) {
                m_ulCurrent = 0L;
                return true;
            }

            return false;
        } 
        
    }; // end of OCTimerAnimation class

} // end of namespace tlib