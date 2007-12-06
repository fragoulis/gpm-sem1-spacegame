#pragma once
#include <ctime>

namespace tlib
{

    const double M_1_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

    class Timer
    {
    private:
        // The time the timer started
        // Default: 0.0
        double m_dStart;

        // The time since timer started. Used for when the timer stops
        // and we want to get the elapsed time for the last run.
        // Default: 0.0
        double m_dElapsed;

        // How long the timer should run for. If zero, timer never stops
        // Default: 0.0
        double m_dDuration;

        // Flag whether its running
        // Default: false
        bool m_bIsRunning;

    public:
        /**
         * Constructor
         */
        Timer();

        /**
         * Duration setter
         */
        void setDuration( double dDuration ) {
            m_dDuration = dDuration;
        }

        /**
         * Return running flag
         */
        bool isRunning() const { return m_bIsRunning; }

        /**
         * Timer controls
         */
        // Starts the timer. If timer is already running is ignored.
        inline void start();

        // Restarts the timer. Resets the elapsed time and continues running.
        inline void restart();

        // Stops the timer
        inline void stop();

        // Checks if timer has expired
        inline bool hasExpired() const;

        /**
         * Returns the time passed since the timer started, in seconds
         */
        inline double elapsedTime() const;

    }; // end of Timer class

    // ----------------------------------------------------------------------------
    // Inline function implementations
    // ----------------------------------------------------------------------------
    void Timer::start() 
    {
        if( m_bIsRunning ) return;

        m_bIsRunning = true;
        m_dStart = clock();
    }

    // ----------------------------------------------------------------------------
    void Timer::stop()
    {
        if( m_bIsRunning )
        {
            m_dElapsed = elapsedTime();
            m_bIsRunning = false;
        }
    }

    // ----------------------------------------------------------------------------
    void Timer::restart() 
    {
        m_bIsRunning = true;
        m_dElapsed = 0;
        m_dStart = clock();
    }

    // ----------------------------------------------------------------------------
    bool Timer::hasExpired() const
    {
        // Check for non-stop timer
        if( m_dDuration < 1e-6 ) return false;

        // Check if elapsed time is greater than the set duration
        if( elapsedTime() >= m_dDuration )
        {
            // If so return true
            return true;
        }

        return false;
    }

    // ----------------------------------------------------------------------------
    double Timer::elapsedTime() const
    {
        return ( clock() - m_dStart ) * M_1_CLOCKS_PER_SEC;
    }

} // end of namespace tlib