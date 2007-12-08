#pragma once
#include "TimeSource.h"

class ClockTimeSource : public TimeSource
{
public:
    feed_t get();
};