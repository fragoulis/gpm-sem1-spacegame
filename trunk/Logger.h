
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Singleton.h"

using namespace std;

#ifdef _DEBUG
#   define _LOG(str) {Logger::Instance().log(str);}
#else
#   define _LOG(str)
#endif

namespace tlib
{

    class Logger : public Singleton<Logger>
    {
        friend Singleton<Logger>;

    private:
        // The file stream
        ofstream m_fsLog;

    public:
        static const char *ERROR_LOG;
        static const char *LOADING_LOG;
        static const char *RUNTIME_LOG;

    private:
        /**
         * Constructors
         */
        Logger();
        Logger( const char *filename );

        /**
         * Destructor
         */
        ~Logger() {}

    public:
        /**
         * Appends an entry to the log
         */
        void log( const string& entry );

        /**
         * Opens the stream for logging
         */
        void open( const char *filename, bool clear = false );

    }; // end of Logger class

} // end of namespace tlib


class BadConversion : public std::runtime_error { 
public:
    BadConversion( const std::string& s )
        : std::runtime_error(s)
    {}
};

template<typename T>
inline std::string toStr( T x )
{
    std::ostringstream o;
    if (!(o << x))
        throw BadConversion("stringify(double)");
    return o.str();
}