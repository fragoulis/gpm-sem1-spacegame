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
    ofstream m_fsTimes;
    ofstream m_fsInputs;

    // The last clock time recorded
    long m_lClockTime;

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
    inline void recordAxis( int iAxis, float fValue );
    inline void recordButton( int iButton, bool bValue );

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
    if( m_fsTimes.is_open() ) return true;

    m_fsTimes.exceptions ( ofstream::eofbit | ofstream::failbit | ofstream::badbit );
    try {
        m_fsTimes.open( m_sFile.c_str() );

        string sInputsFile = m_sFile;
        size_t pos = m_sFile.find_last_of('.');
        m_fsInputs.open( (m_sFile.insert(pos,"_inputs")).c_str() );
    }
    catch( ofstream::failure e ) {
    }

    if( !m_fsTimes.is_open() || !m_fsInputs.is_open() ) return false;

    return ( m_bIsOn = true );
}

// ----------------------------------------------------------------------------
void Recorder::stop()
{
    m_fsTimes.close();
    m_bIsOn = false;
}

// ----------------------------------------------------------------------------
void Recorder::record( long lClockTime )
{
    m_fsTimes << lClockTime << " ";
    //m_lClockTime = lClockTime;
}

// ----------------------------------------------------------------------------
void Recorder::recordAxis( int iAxis, float fValue )
{
    m_fsInputs << iAxis << " " << fValue << " ";
}

// ----------------------------------------------------------------------------
void Recorder::recordButton( int iButton, bool bValue )
{
    m_fsInputs << iButton << " " << ((bValue)?1:0) << " ";
}