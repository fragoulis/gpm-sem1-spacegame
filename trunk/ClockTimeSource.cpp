#include "ClockTimeSource.h"
#include <ctime>
extern const double M_1_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

feed_t ClockTimeSource::get()
{
    return clock();
}