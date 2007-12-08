#include "Clock.h"
#ifdef _DEBUG
#  define NDEBUG
#endif
#include <cassert>
#include "Recorder.h"
extern const double M_1_CLOCKS_PER_SEC;

Clock::Clock():
m_dCurrentTime(0.0),
m_lStart(0),
m_TimeSrc(0)
{
    // Zero all values
    for( int i=0; i<FRAMES_SAVED; ++i ) {
        m_vdFrameTimes[i] = 0.0;
    }
}

Clock::~Clock()
{
    // Detele time source
    delete m_TimeSrc;
    m_TimeSrc = 0;
    
    // Clear timer array
    TimerArray::iterator i;
    for( i=m_vTimers.begin(); i!=m_vTimers.end(); ++i ) 
    {
        if( *i ) 
        {
            delete *i;
            *i=0;
        }
    }
}

// ----------------------------------------------------------------------------
void Clock::FrameStep()
{
    assert(m_TimeSrc!=0);

    m_lNow = m_TimeSrc->get();

    // Update current time
    m_dPreviousTime = m_dCurrentTime;
    m_dCurrentTime = ( m_lNow - m_lStart ) * M_1_CLOCKS_PER_SEC;

    // Compute average time delta
    _updateDeltaTime();

    // Update application timers
    TimerArray::const_iterator i;
    for( i=m_vTimers.begin(); i!=m_vTimers.end(); ++i ) 
    {
        (*i)->update( m_dDeltaTime );
    }
}

// ----------------------------------------------------------------------------
void Clock::Start( TimeSource *p_TimeSrc )
{
    assert(p_TimeSrc!=0);

    // Makes sure we don't restart the application clock
    if( m_lStart > 0 ) return;

    m_TimeSrc = p_TimeSrc;

    // Save current time
    m_lStart = m_TimeSrc->get();
}

// ----------------------------------------------------------------------------
Timer* Clock::GetTimer()
{
    Timer* newTimer = new Timer;
    m_vTimers.push_back(newTimer);
    return newTimer;
}

// ----------------------------------------------------------------------------
void Clock::_updateDeltaTime()
{
    // Push every value up one index
    for( int i=FRAMES_SAVED-1; i>0; --i )
        m_vdFrameTimes[i] = m_vdFrameTimes[i-1];

    // Save the last time delta
    m_vdFrameTimes[0] = m_dCurrentTime - m_dPreviousTime;

    // Calculate the average of the N previous time deltas
    double dSum = 0.0;
    for( int i=0; i<FRAMES_SAVED; ++i ) {
        dSum += m_vdFrameTimes[i];
    }
    m_dDeltaTime = dSum / FRAMES_SAVED;
}