#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace tlib
{
    // ------------------------------------------------------------------------
    // Returns a random integer between min and max
    // ------------------------------------------------------------------------
    inline int randInt( const int min, const int max )
    {
        if( min == max ) return min;
        //if( min > max ) return 0;
        //if( min < -INT_MAX ) min = -INT_MAX;
        //if( max > INT_MAX ) max = INT_MAX;

        return ( ( rand()%(max-min) ) + min );
    }

    // ------------------------------------------------------------------------
    // Returns a random float between min and max
    // ------------------------------------------------------------------------
    inline float randFloat( const float min, const float max )
    {
        // convert limits to integers
        int imin = (int)(1000.0f * min);
        int imax = (int)(1000.0f * max);

        if( imin < -INT_MAX  || 
            imax > INT_MAX   ||
            imin > imax )     return 0;

        // get a random integer
        int res = randInt( imin, imax );

        // convert and return it
        return (float)res * 0.0001f;
    }

    // ------------------------------------------------------------------------
    // Returns a random integer between -range and range
    // ------------------------------------------------------------------------
    inline int randInt( const int range )
    {
        if( range < 0         || 
            range > INT_MAX ) return 0;

        return ( ( rand()%(range+range) ) - range );
    }

    // ------------------------------------------------------------------------
    // Returns a random float between 0 and 1 with 3 digits precision
    // ------------------------------------------------------------------------
    inline float randUnit()
    {
        return (float)( ( rand()%1000 ) * 0.0001f );
    }
}