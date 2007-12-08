#pragma once
#include "Singleton.h"
#include "TimeSource.h"
#include "Timer.h"
#include <vector>
using std::vector;

// Number of frames saved to compute the average time delta
static const int FRAMES_SAVED = 5;

class Timer;
typedef vector<Timer*> TimerArray;

class Clock : public Singleton<Clock>
{
    friend Singleton<Clock>;
private:
    // The clock's time source
    TimeSource *m_TimeSrc;
    
    // The time the timer started
    feed_t m_lStart;
    // The curent feed
    feed_t m_lNow;

    // The current time is the time passed since the start
    // of the application in seconds
    double m_dCurrentTime;
    // The previous time
    double m_dPreviousTime;

    // This array holds the times of the last N feeds, giving 
    // us the opportunity to calculate a more accurate delta time.
    // The first item in the array is the last time step occured 
    // [the last time delta].
    double m_vdFrameTimes[FRAMES_SAVED];

    // The average time delta
    double m_dDeltaTime;

    // An array of child timers
    TimerArray m_vTimers;

public:
    /**
     * Accessors
     */
    feed_t getCurrentFeed() const { return m_lNow; }
    double getCurrentTime() const { return m_dCurrentTime; }
    float getDeltaTime() const { return (float)m_dDeltaTime; }
    double getDeltaTimeDouble() const { return m_dDeltaTime; }
    double getLastDeltaTime() const { 
        return m_vdFrameTimes[0]; 
    }

    /**
     * This function updates the current time, the time detla
     * and all associated timers
     */
    void FrameStep();

    /**
     * Initiates the clock. We must pass a time source object
     * to associate with the clock.
     */
    void Start( TimeSource *p_TimeSrc );

    /**
     * Returns a pointer to a new timer.
     */
    Timer* GetTimer();

private:
    /**
     * Constructor/Destructor
     */
    Clock();
    ~Clock();

    /**
     * Calculates the average time delta
     */
    void _updateDeltaTime();

}; // end of Clock class