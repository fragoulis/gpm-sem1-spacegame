#include "Recorder.h"

Recorder::Recorder():
m_bIsRecord(false),
m_bIsReplay(false)
{}

Recorder::~Recorder()
{}

// ----------------------------------------------------------------------------
void Recorder::setFiles( const string &sFile ) {
    m_sTimes = sFile;
    m_sInputs = sFile;
    size_t pos = m_sTimes.find_last_of('.');
    m_sInputs.insert(pos,"_inputs");
}

// ----------------------------------------------------------------------------
bool Recorder::openInputsForReplay()
{
    m_ifsInputs.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
    try {
        m_ifsInputs.open( m_sInputs.c_str() );
    }
    catch( ifstream::failure e ) {
    }

    if( !m_ifsInputs.is_open() ) return false;

    m_bIsReplay = true;
    return true;
}