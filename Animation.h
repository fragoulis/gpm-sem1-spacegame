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

    public:
        /**
         * Constructor
         */
        IOCAnimation(): m_bIsOn(0), m_bRepeat(0) 
        {}
        IOCAnimation( bool bRepeat ): m_bIsOn(0), m_bRepeat(bRepeat)
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
         * Getter/Setter for the animation's repeat flag
         */
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

            onStart();
            unfreeze();
        }

        /**
         * Terminates the animation
         */
        void stop() 
        {
            freeze();
            onStop();
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
                if( m_bRepeat ) start();
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