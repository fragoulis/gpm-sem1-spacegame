#pragma once
#include "TimeSource.h"
#include <fstream>
using std::ifstream;

class FileTimeSource : public TimeSource
{
private:
    // The input stream
    ifstream m_fsIn;

public:
    FileTimeSource( const char *sFile );
    feed_t get();
};