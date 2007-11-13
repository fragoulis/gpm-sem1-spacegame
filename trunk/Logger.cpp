#include "Logger.h"

namespace tlib
{
    const char *Logger::ERROR_LOG    = "log.errors.txt";
    const char *Logger::LOADING_LOG  = "log.loading.txt";
    const char *Logger::RUNTIME_LOG  = "log.runtime.txt";

    Logger::Logger() {}
    Logger::Logger( const char *filename ) 
    {
#ifdef _DEBUG
        m_fsLog.open( filename, ios_base::app );
#endif
    }

    void Logger::log( const string& entry )
    {
#ifdef _DEBUG
        if( m_fsLog.is_open() )
            m_fsLog << entry << endl;
        else 
            cout << entry << endl;
#endif
    }

    /**
     * Opens the stream for logging
     */
    void Logger::open( const char *filename, bool clear )
    {
        if( clear )
        {
            m_fsLog.open( filename );
        } else
        {
            m_fsLog.open( filename, ios_base::app  );
            log( "\nStarting..." );
        }
    }

} // end of namespace tlib