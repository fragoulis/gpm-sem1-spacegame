#pragma once
#include "Animation.h"

namespace tlib
{

    class OCTimerAnimation : public IOCAnimation
    {
    private:
        // How long the animation will last in miliseconds
        unsigned int m_uiDuration;

        // The current time
        unsigned int m_uiCurrent;

    public:
        /**
         * Constructor
         */
        OCTimerAnimation(): 
            m_uiDuration(0), 
            m_uiCurrent(0)
        {}
        OCTimerAnimation( bool bRepeat ): 
            m_uiDuration(0), 
            m_uiCurrent(0),
            IOCAnimation( bRepeat )
        {}

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
        unsigned long getDuration() const { return m_uiDuration; }
        void setDuration( unsigned long ulDuration ) { 
            m_uiDuration = ulDuration; 
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
        virtual void OnTimerChange( unsigned int uiCurrentTime,
                                    unsigned int uiDuration ) const = 0;

    private:
        void onUpdate() 
        { 
            m_uiCurrent++; 
            OnTimerChange( m_uiCurrent, m_uiDuration );
        }

        bool condition()
        {
            if( m_uiCurrent >= m_uiDuration ) {
                m_uiCurrent = 0L;
                return true;
            }

            return false;
        } 
        
    }; // end of OCTimerAnimation class

} // end of namespace tlib