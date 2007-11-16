#pragma once
#include "component.h"

namespace tlib
{

    class IOCAnimation : public IComponent
    {
    private:
        // Whether the animation is active or not
        bool m_bIsOn;

        // Whether the animation should repeat or not
        bool m_bRepeat;

        // Whether the animation has ended
        // If this flag is true, an animation can only restart if
        // the repeat flag is on
        bool m_bIsDone;

    public:
        /**
         * Constructor
         */
        IOCAnimation(): 
            m_bIsOn(0), 
            m_bRepeat(0), 
            m_bIsDone(0)
        {}
        IOCAnimation( bool bRepeat ): 
            m_bIsOn(0), 
            m_bRepeat(bRepeat), 
            m_bIsDone(0)
        {}

        /**
         * Destructor
         */
        virtual ~IOCAnimation(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("animation"); 
        }

        /**
         * Getter/Setter for the animation's flags
         */
        bool isOn() const { return m_bIsOn; }
        bool isDone() const { return m_bIsDone; }
        bool getRepeat() const { return m_bRepeat; }
        void setRepeat( bool bRepeat ) { 
            m_bRepeat = bRepeat; 
        }

        /**
         * Initiates the animation
         */
        void start() 
        {
            // If animation is already running avoid reseting it
            if( m_bIsOn ) return;
            if( m_bIsDone ) return;

            onStart();
            unfreeze();
        }

        /**
         * Restarts the animation
         */
        void restart()
        {
            m_bIsDone = false;
            start();
        }

        /**
         * Terminates the animation
         */
        void stop() 
        {
            freeze();
            onStop();
            m_bIsDone = true;
        }

        /**
         * Updates the animation
         */
        void update() {
            // Return if animation is either paused or inactive
            if( !m_bIsOn ) return;

            onUpdate();
            
            // The animation stops when the custom condition
            // comes true
            if( condition() ) 
            {
                freeze();

                // If the repeat flag is on start over
                if( m_bRepeat ) restart();
                else stop();
            }
        } // end of update()

        /**
         * Pauses the animation
         */
        void freeze() { m_bIsOn = false; }

        /**
         * Unpauses the animation
         */
        void unfreeze() { m_bIsOn = true; }

    protected:
        /**
         * Animation control callbacks
         */
        virtual void onStart()   = 0;
        virtual void onStop()    = 0;
        virtual void onUpdate()  = 0;
        virtual bool condition() = 0;

    }; // end of IOCAnimation class

} // end of namespace tlib