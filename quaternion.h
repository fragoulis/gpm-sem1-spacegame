 
#pragma once

#include <iostream>
#include "math.h"

using std::ostream;
using std::istream;

namespace tlib 
{

    // Let the Quaternion class know of the Vector3 class
    template<typename T> class Vector3;

    template<typename T>
    class Quaternion
    {
    private:
        T _wxyz[4];

    public:
        /**
         * Constructors
         */
        Quaternion() { w((T)1.0); xyz((T)0); }
        Quaternion( const T wxyz[] ) { this->wxyz( wxyz ); }
        Quaternion( const T w, const T x, const T y, const T z ) { 
            wxyz( w, x, y, z); 
        }
        Quaternion( const T w, const Vector3<T> &vec ) {
            this->w(w); xyz(vec);
        }

        // let compiler create copy constructor, '=' assignment 
        // operator and destructor automatically

        /**
         * Returns the w component
         */
        T w() const { return _wxyz[0]; }

        /**
         * Returns the x component
         */
        T x() const { return _wxyz[1]; }

        /**
         * Returns the y component
         */
        T y() const { return _wxyz[2]; }

        /**
         * Returns the z component
         */
        T z() const { return _wxyz[3]; }


        /**
         * Returns all quaternion components
         */
        const T* wxyz() const { return _wxyz; }

        /**
         * Returns the imaginary part of the quaternion in vector format
         */
        Vector3<T> xyz() { return Vector3<T>( &_wxyz[1] ); }

        /**
         * Assigns a value to the   component
         */
        void w( const T w ) { _wxyz[0] = w; }

        /**
         * Assigns a value to the x component
         */
        void x( const T x ) { _wxyz[1] = x; }

        /**
         * Assigns a value to the y component
         */
        void y( const T y ) { _wxyz[2] = y; }

        /**
         * Assigns a value to the z component
         */
        void z( const T z ) { _wxyz[3] = z; }

        /**
         * Assigns the same value to all components
         */
        void wxyz( const T val ) 
        { 
            w( val ); x( val ); y( val ); z( val );
        }

        /**
         * Assigns a value to each component
         */
        void wxyz( const T w, const T x, const T y, const T z ) 
        { 
            this->w( w ); this->x( x ); this->y( y ); this->z( z );
        }
        
        /**
         * Copies values from another quaternion 
         */
        void wxyz( const Quaternion<T> &quat ) 
        { 
            this->wxyz( quat.w(), quat.x(), quat.y(), quat.z() );
        }

        /**
         * Assigns a value to each component
         */
        void wxyz( const T wxzy[] ) 
        { 
            this->wxyz( wxyz[0], wxyz[1], wxyz[2], wxyz[3] );
        }

        /**
         * Assigns the same value to imaginary components
         */
        void xyz( const T val ) 
        { 
            x( val ); y( val ); z( val );
        }

        /**
         * Assigns a vector to the imaginary part
         */
        void xyz( const Vector3<T> &vec ) 
        { 
            x( vec.x() ); y( vec.y() ); z( vec.z() );
        }

        /**
         * Creates a rotation quaternion out of an axis and an angle
         */
        void toRotation( float w, const Vector3<T> &xyz );

        /**
         * Makes a rotation quaternion out of the existing data
         */
        void toRotation();

        /**
         * Makes a rotation quaternion out of euler angles
         */
        void toRotation( const float fPitch, const float fYaw,
                         const float fRoll );
        
        /**
         * Converts the quaternion into a 4x4 matrix
         */
        void toMatrix( float *m ) const;

        /**
         * Normalizes the quaternion 
         */
        void normalize();

        /**
         * Returns the length of the quaternion
         */
        float length() const { return sqrt( squaredLength() ); }

        /**
         * Returns the squared length of the quaternion
         */
        float squaredLength() const 
        { 
            return ( _wxyz[0]*_wxyz[0] + 
                     _wxyz[1]*_wxyz[1] + 
                     _wxyz[2]*_wxyz[2] + 
                     _wxyz[3]*_wxyz[3] );
        }

        /**
         * Conjugates the quaternion
         */
        Quaternion<T> conjugate() const
        { 
            return Quaternion<T>( w(), -x(), -y(), -z() ); 
        }
       
        /**
         * Conjugates the quaternion
         */
        Quaternion<T> operator~() const { return conjugate(); }

        /**
         * Multiplies the two quaternions
         */
        Quaternion<T> multiply( const Quaternion<T> &quat ) const
        {
            float
                A = (w() + x())*(quat.w() + quat.x()),
                B = (z() - y())*(quat.y() - quat.z()),
                C = (w() - x())*(quat.y() + quat.z()), 
                D = (y() + z())*(quat.w() - quat.x()),
                E = (x() + z())*(quat.x() + quat.y()),
                F = (x() - z())*(quat.x() - quat.y()),
                G = (w() + y())*(quat.w() - quat.z()),
                H = (w() - y())*(quat.w() + quat.z());

            return Quaternion<T>( B + (-E - F + G + H ) * 0.5f,
                                  A - ( E + F + G + H ) * 0.5f,
                                  C + ( E - F + G - H ) * 0.5f,
                                  D + ( E - F - G + H ) * 0.5f );
        }

        /**
         * Multiplies the two quaternions
         */
        Quaternion<T> operator*( const Quaternion<T> &quat ) const
        {
            return multiply( quat );
        }

        /**
         * Multiplies the two quaternions
         */
        Quaternion<T>& operator*=( const Quaternion<T> &quat )
        {
            *this = multiply( quat );

            return *this;
        }

        void write( ostream &cout ) const
        {
            cout << "( " << w() << ", "
                         << x() << ", "
                         << y() << ", "
                         << z() << " )";
        }

        void slerp( const Quaternion<T>& qTo, const float fTime, 
                    Quaternion<T>& res ) const;

        static Quaternion<T> Identity() {
            return Quaternion<T>;
        }

    }; // end of Quaternion class

    /** Members functions **/
    template<typename T>
    void Quaternion<T>::toRotation( float w, const Vector3<T> &xyz )
    {
        w *= 0.5f; // angle / 2
        const T mysin = sin(w);

        // Setup rotation quaternion
        this->w( cos(w) );
        this->x( xyz.x() * mysin );
        this->y( xyz.y() * mysin );
        this->z( xyz.z() * mysin );

    } // end toRotation()

    template<typename T>
    void Quaternion<T>::toRotation( const float fPitch, const float fYaw,
                                    const float fRoll )
    {
        float
            cr = cos( fRoll * 0.5f ),
            cp = cos( fPitch * 0.5f ),
            cy = cos( fYaw * 0.5f ),

            sr = sin( fRoll * 0.5f ),
            sp = sin( fPitch * 0.5f ),
            sy = sin( fYaw * 0.5f ),
    	
            cpcy = cp * cy,
            spsy = sp * sy;


        w( cr * cpcy + sr * spsy );
        x( sr * cpcy - cr * spsy );
        y( cr * sp * cy + sr * cp * sy );
        z( cr * cp * sy - sr * sp * cy );

    } // end toRotation()

    template<typename T>
    void Quaternion<T>::toRotation()
    {
        w( w() * 0.5f ); // angle / 2
        const T mysin = sin( w() );

        // Setup rotation quaternion
        w( cos( w() ) );
        x( x() * mysin );
        y( y() * mysin );
        z( z() * mysin );

    } // end toRotation()

    template<typename T>
    void Quaternion<T>::normalize()
    {
        // get vector length
        float len = length();
        
        // if lenth already unit then return :)
        //if( fcmp( len, 1.0f ) ) return;

        // if length is zero, then the components are all zero
        if( len < 1e-6 )
        {
            wxyz((T)0);
            return;
        }
        
        // choose to divide once and multiply thrice
        // instead of just dividing three times
        len = 1.0f / len;
        _wxyz[0] *= len;
        _wxyz[1] *= len;
        _wxyz[2] *= len;
        _wxyz[3] *= len;

    } // end normalize()

    template<typename T>
    void Quaternion<T>::toMatrix( float *m ) const
    {
        if( !m ) return;

        // First row
	    m[ 0] = 1.0f - 2.0f * ( y() * y() + z() * z() );
	    m[ 1] = 2.0f * (x() * y() + z() * w());
	    m[ 2] = 2.0f * (x() * z() - y() * w());
	    m[ 3] = 0.0f;
    	
	    // Second row
	    m[ 4] = 2.0f * ( x() * y() - z() * w() );
	    m[ 5] = 1.0f - 2.0f * ( x() * x() + z() * z() );
	    m[ 6] = 2.0f * (z() * y() + x() * w() );
	    m[ 7] = 0.0f;

	    // Third row
	    m[ 8] = 2.0f * ( x() * z() + y() * w() );
	    m[ 9] = 2.0f * ( y() * z() - x() * w() );
	    m[10] = 1.0f - 2.0f * ( x() * x() + y() * y() );
	    m[11] = 0.0f;

	    // Fourth row
	    m[12] = 0;
	    m[13] = 0;
	    m[14] = 0;
	    m[15] = 1.0f;

    } // end toMatrix()

    template<typename T>
    ostream& operator<<( ostream &cout, const Quaternion<T> &quat )
    {
        quat.write( cout );
        return cout;
    }

    /**
     * Interpolates between two given quaternions
     */
    template<typename T>
    void Quaternion<T>::slerp( const Quaternion<T>& qTo, 
                               const float fTime, 
                               Quaternion<T>& qRes ) const
    {
        float   to1[4];

        // calc cosine
        float cosom = x() * qTo.x() + 
                      y() * qTo.y() + 
                      z() * qTo.z() + 
                      w() * qTo.w();

        // adjust signs (if necessary)
        if( cosom < 1e-6f ) { 
            cosom = -cosom; 
            to1[0] = -qTo.x();
            to1[1] = -qTo.y();
            to1[2] = -qTo.z();
            to1[3] = -qTo.w();
        } else  {
            to1[0] = qTo.x();
            to1[1] = qTo.y();
            to1[2] = qTo.z();
            to1[3] = qTo.w();
        }

        // calculate coefficients
        float  omega, sinom, scale0, scale1;
        if( ( 1.0f - cosom ) > 1e-3f ) {
            // standard case (slerp)
            omega   = acos(cosom);
            sinom   = 1.0f / sin(omega);
            scale0  = sin( ( 1.0f - fTime ) * omega ) * sinom;
            scale1  = sin( fTime * omega ) * sinom;
        } else {        
        // "from" and "to" quaternions are very close 
        //  ... so we can do a linear interpolation
                scale0 = 1.0f - fTime;
                scale1 = fTime;
        }

        // calculate final values
        qRes.x( scale0 * x() + scale1 * to1[0] );
        qRes.y( scale0 * y() + scale1 * to1[1] );
        qRes.z( scale0 * z() + scale1 * to1[2] );
        qRes.w( scale0 * w() + scale1 * to1[3] );
    }

    typedef Quaternion<int>     Quati;
    typedef Quaternion<float>   Quatf;
    typedef Quaternion<double>  Quatd;

} // end of tlib namespace