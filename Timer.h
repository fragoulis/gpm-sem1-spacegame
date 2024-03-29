#pragma once
#define E_ZERO 1e-8

class Clock;
class Timer
{
    friend Clock;
private:
    // The number of seconds before the timer stops 
    // automatically.
    // This can be zero for an infinite timer.
    double m_dDuration;

    // The current [local] time, represents the seconds
    // passed since timer has started
    double m_dElapsedTime;

    // Flag whether its running
    // Default: false
    bool m_bIsRunning;

    // This variable defines how consistent is this timer
    // with the clock. By default is set to 1.0, which means
    // that will update exactly the time delta.
    // Default: 1.0f
    float m_fScale;

public:
    /**
     * Constructor
     */
    Timer();

    /**
     * Mutators
     */
    void setDuration( double dDuration ) {
        m_dDuration = dDuration;
    }

    /**
     * Accessors
     */
    bool isRunning() const { return m_bIsRunning; }
    double getElapsedTime() const { return m_dElapsedTime; }

    /**
     * Mutators
     */
    void setScale( float fScale ) { m_fScale = fScale; }

    /**
     * Starts the timer. If timer is already running is ignored.
     */
    inline void start();

    /**
     * Restarts the timer. Resets the elapsed time and continues running.
     */
    inline void restart();

    /**
     * Stops the timer. Does not reset it. So basicaly can be considered as
     * pause as well.
     */
    inline void stop();

    /**
     * Return a how close the timer is to the end, in the form
     * of ratio
     */
    inline double ratio() const;

private:
    /**
     * Updates the timer's values
     */
    inline void update( double dTimeDelta );

}; // end of Timer class

// ----------------------------------------------------------------------------
// Inline function implementations
// ----------------------------------------------------------------------------
void Timer::start() 
{
    if( m_bIsRunning ) return;

    restart();
}

// ----------------------------------------------------------------------------
void Timer::stop()
{
    m_bIsRunning = false;
}

// ----------------------------------------------------------------------------
void Timer::restart() 
{
    m_bIsRunning = true;
    m_dElapsedTime = 0.0;
}

// ----------------------------------------------------------------------------
void Timer::update( double dTimeDelta )
{
    // Update elapsed time
    m_dElapsedTime += dTimeDelta * double(m_fScale);

    // If this is a non-stop time, return now
    if( m_dDuration < E_ZERO ) return;

    // Otherwise, check whether the elapsed time has exceeded
    // the duration. If so, stop the timer.
    if( m_dElapsedTime >= m_dDuration )
    {
        stop();
    }
}

// ----------------------------------------------------------------------------
double Timer::ratio() const
{
    if( m_dDuration < E_ZERO ) 
        return 1.0;

    return m_dElapsedTime / m_dDuration;
}