#pragma once

// Typedef the source's return type here for future flexibility
// We call it 'feed' in the sence that the source feeds the clock
// with the new time...
typedef long feed_t;

/**
 * This interface connects the central application clock with
 * a source to update its time from.
 */
class TimeSource
{
public:
    virtual ~TimeSource(){}
    virtual feed_t get() = 0;
};