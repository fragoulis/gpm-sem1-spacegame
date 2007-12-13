#pragma once
#include "Singleton.h"
#include <fstream>
#include <string>
using namespace std;

class Recorder  : public Singleton<Recorder>
{
    friend Singleton<Recorder>;
private:
    // Tells whether we are reconding/replaying
    bool m_bIsRecord, m_bIsReplay;

    // File to record to
    string m_sTimes;
    string m_sInputs;

    // The output stream
    ofstream m_ofsTimes;
    ofstream m_ofsInputs;
    ifstream m_ifsInputs;

    // The last clock time recorded
    long m_lClockTime;

public:
    /**
     * Accesors/Mutators
     */
    bool isRecordOn() const { return m_bIsRecord; }
    bool isReplayOn() const { return m_bIsReplay; }
    void setFiles( const string &sFile );
	bool isOpen() const { return m_ifsInputs.is_open(); }

    /**
     * Records the clock feeds into a text file
     */
    inline void record( long lClockTime );
    inline void recordAxis( float fValues[], int iCount );
    inline void recordButton( bool bValues[], int iCount );
    inline void flush();
    inline void replayAxis( float fValues[], int iCount );
    inline void replayButton( bool bValues[], int iCount );
	inline void endReplay();

    /**
     * Starts/Stops the recording/replay
     */
    inline bool start();
    inline void stop();
    bool openInputsForReplay();

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
    if( m_ofsTimes.is_open() ) return true;

    m_ofsTimes.exceptions ( ofstream::eofbit | ofstream::failbit | ofstream::badbit );
    m_ofsInputs.exceptions ( ofstream::eofbit | ofstream::failbit | ofstream::badbit );
    try {
        m_ofsTimes.open( m_sTimes.c_str() );
        m_ofsInputs.open( m_sInputs.c_str() );
    }
    catch( ofstream::failure e ) {
    }

    if( !m_ofsTimes.is_open() || !m_ofsInputs.is_open() ) return false;

    return ( m_bIsRecord = true );
}

// ----------------------------------------------------------------------------
void Recorder::stop()
{
    m_ofsTimes.close();
    m_ofsInputs.close();
    m_bIsRecord = false;
}

// ----------------------------------------------------------------------------
void Recorder::record( long lClockTime )
{
    m_ofsTimes << lClockTime << " ";
}

// ----------------------------------------------------------------------------
void Recorder::flush()
{
    m_ofsInputs << endl;
    m_ofsInputs.flush();
}

// ----------------------------------------------------------------------------
void Recorder::recordAxis( float fValues[], int iCount )
{
	for( int i=0; i<iCount; ++i )
        m_ofsInputs << fValues[i] << " ";
}

// ----------------------------------------------------------------------------
void Recorder::recordButton( bool bValues[], int iCount )
{
	for( int i=0; i<iCount; ++i )
        m_ofsInputs << ((bValues[i])?1:0) << " ";
}

// ----------------------------------------------------------------------------
void Recorder::replayAxis( float fValues[], int iCount )
{
    for( int i=0; i<iCount; ++i )
        m_ifsInputs >> fValues[i];
}

// ----------------------------------------------------------------------------
void Recorder::replayButton( bool bValues[], int iCount )
{
    for( int i=0; i<iCount; ++i )
        m_ifsInputs >> bValues[i];

    if( !m_ifsInputs.good() ) {
		endReplay();
	}
}

// ----------------------------------------------------------------------------
void Recorder::endReplay()
{
    m_ifsInputs.close();
    m_bIsReplay = false;
}