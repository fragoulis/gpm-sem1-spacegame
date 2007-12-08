#pragma once
#include "Singleton.h"
#include <fstream>
#include <string>
using namespace std;

class Recorder  : public Singleton<Recorder>
{
    friend Singleton<Recorder>;
private:
    // Tells whether we are reconding
    bool m_bIsOn;

    // File to record to
    string m_sFile;

    // The output stream
    ofstream m_fsOut;

public:
    /**
     * Accesors/Mutators
     */
    bool isOn() const { return m_bIsOn; }
    void setFile( const string &sFile ) {
        m_sFile = sFile;
    }

    /**
     * Records the clock feeds into a text file
     */
    inline void record( long lClockTime );

    /**
     * Starts/Stops the recording
     */
    inline bool start();
    inline void stop();

private:
    /**
     * Constructor/Destructor
     */
    Recorder();
    ~Recorder();
};

// ----------------------------------------------------------------------------
// Inline function implementations
// ----------------------------------------------------------------------------
bool Recorder::start()
{
    m_fsOut.open( m_sFile.c_str() );
    if( !m_fsOut.is_open() )
        return false;

    return ( m_bIsOn = true );
}

// ----------------------------------------------------------------------------
void Recorder::stop()
{
    m_fsOut.close();
    m_bIsOn = false;
}

// ----------------------------------------------------------------------------
void Recorder::record( long lClockTime )
{
    m_fsOut << lClockTime << " ";
}