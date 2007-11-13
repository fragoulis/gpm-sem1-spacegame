
#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

#include "quaternion.h"
#include "vector3.h"

#define FZERO_E 1e-6

static const double M_DEG_TO_RAD = 0.01745329;
static const double M_RAD_TO_DEG = 57.29577951;

namespace tlib
{
    // ------------------------------------------------------------------------
    // Normalizes a given vector and returns copy of that
    // This can be altered to create a heap object intead of a stack object
    // just to avoid the overhead of copying the returned object
    // ------------------------------------------------------------------------
    template<typename T>
    Vector3<T> normalize( const Vector3<T> &vec )
    {
        const float length = 1 / vec.length();

        return Vector3<T>( vec.x() * length,
                           vec.y() * length,
                           vec.z() * length );
    }

    // ------------------------------------------------------------------------
    // Calculates and returns the dot product of two given normalized vectors
    // ------------------------------------------------------------------------
    template<typename T>
    double dot( const Vector3<T> &vec1, const Vector3<T> &vec2 )
    {
        return  vec1.x() * vec2.x() + 
                vec1.y() * vec2.y() + 
                vec1.z() * vec2.z();
    }

    // ------------------------------------------------------------------------
    // Calculates and returns the cross product of two given normalized vectors
    // ------------------------------------------------------------------------
    template<typename T>
    Vector3<T> cross( const Vector3<T> &vec1, const Vector3<T> &vec2 )
    {
        return Vector3<T>( vec1.y() * vec2.z() - vec1.z() * vec2.y(),
                           vec1.z() * vec2.x() - vec1.x() * vec2.z(),
                           vec1.x() * vec2.y() - vec1.y() * vec2.x() );
    }

    template<typename T>
    void cross( const Vector3<T> &vec1, const Vector3<T> &vec2, Vector3<T> &res )
    {
        res.xyz( vec1.y() * vec2.z() - vec1.z() * vec2.y(),
                 vec1.z() * vec2.x() - vec1.x() * vec2.z(),
                 vec1.x() * vec2.y() - vec1.y() * vec2.x() );
    }

    /**
     * Compares two floats and returns true if the are significantly equal
     */
    inline bool fcmp( float value, float to )
    {
        return ( ( fabs(value) > to-FZERO_E ) && 
                 ( fabs(value) < to+FZERO_E ) );
    }

    // ------------------------------------------------------------------------
    // Returns a random integer between min and max
    // ------------------------------------------------------------------------
    inline int randInt( const int min, const int max )
    {
        if( min < -INT_MAX  || 
            max > INT_MAX   ||
            min > max )     return 0;

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

} // end of tlib namespace