#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#   define _CLEAR_LOG {Logger log;}
#   define _LOG(str) {Logger log(str);}
#else
#   define _CLEAR_LOG
#   define _LOG(str)
#endif

namespace tlib
{

    class Logger
    {
    public:
        /**
         * Constructor
         */
        Logger();
        Logger( const string& entry );

    private:
        static const char *LOG_FILE;

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