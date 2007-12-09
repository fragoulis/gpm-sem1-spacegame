#include "Logger.h"

namespace tlib
{
    const char *Logger::LOG_FILE = "logfile.txt";

    Logger::Logger() 
    {
        ofstream log( LOG_FILE );
    }

    Logger::Logger( const string& entry ) 
    {
        ofstream log( LOG_FILE, ios_base::app );

        if( log.is_open() )
            log << entry << endl;

        cout << entry << endl;
    }

} // end of namespace tlib