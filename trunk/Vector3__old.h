#pragma once
#include <iostream>
#include "math.h"

using std::ostream;
using std::istream;

namespace tlib 
{

    template<typename T>
    class Vector3
    {
    private:
        // The vector's components in array format
        T _xyz[3];

        // The vector's length
        // This will help us simplify the normalization of the vector,
        // basically avoid normalization if length is already a unit
        float m_fLength;

    public:
        /**
         * Constructors
         * Here we leave the compiler to create the copy constructor, 
         * the assignment operator and the destructor
         */
        Vector3() {}
        Vector3( const T xyz[] ) { this->xyz( xyz ); }
        Vector3( const T x, const T y, const T z ) { xyz( x, y, z ); }
        
        /**
         * Sets all components to zero
         */
        void reset() { x((T)0); y((T)0); z((T)0); }

        /**
         * Returns the x component
         */
        const T x() const { return _xyz[0]; }

        /**
         * Returns the y component
         */
        const T y() const { return _xyz[1]; }

        /**
         * Returns the x component
         */
        const T z() const { return _xyz[2]; }

        /**
         * Returns all components
         */
        const T* xyz() const { return _xyz; }

        /** 
         * Assigns a value to x component
         */
        void x( const T x ) { _xyz[0] = x; }

        /** 
         * Assigns a value to y component
         */
        void y( const T y ) { _xyz[1] = y; }

        /** 
         * Assigns a value to x component
         */
        void z( const T z ) { _xyz[2] = z; }

        /**
         *  Assigns a value to each component
         */
        void xyz( const T x, const T y, const T z ) { 
            this->x(x); this->y(y); this->z(z);
        }

        /**
         * Copies the values of the given array
         */
        void xyz( const T xyz[] ) { 
            x(xyz[0]); y(xyz[1]); z(xyz[2]);
        }
        
        /**
         * Copies the values of the given vector
         */
        void xyz( const Vector3<T> &vec ) { 
            x(vec.x()); y(vec.y()); z(vec.z());
        }

        /**
         * Returns the length of the vector
         */
        float length() const
        { 
            // Return updated length
            return (float)sqrt( squaredLength() );
        }

        /**
         * Returns the squared length of the vector
         */
        float squaredLength() const { 
            return ( x()*x() + y()*y() + z()*z() );
        }

        /**
         * Calculates the dot product
         */
        float dot( const Vector3<T> &vec ) const
        {
            return ( x()*vec.x() + y()* vec.y() + z()*vec.z() );
        }

        /**
         * Calculates the cross product
         */
        Vector3<T> cross( const Vector3<T> &vec2 ) const
        {
            return Vector3<T>( y() * vec2.z() - z() * vec2.y(),
                               z() * vec2.x() - x() * vec2.z(),
                               x() * vec2.y() - y() * vec2.x() );
        }

        /**
         * Normalizes the vector
         */
        void normalize();

        /** 
         * Saves the normalized components to the given vector
         */
        void normalize( Vector3<T> &vec ) const;

        /**
         * Rotates the vector by angle radians about a given axis
         * and returns the resulting rotated vector
         */
        Vector3<T> rotate( const float fAngle, const Vector3<T> &vAxis ) const;

        /**
         * Applies a rotation in the form of quaternion
         * and returns the resulting rotated vector
         */
        Vector3<T> rotate( const Quaternion<T> &qRotation ) const;

        /**
         * Rotates the vector by angle radians about a given axis
         */
        void selfRotate( const float fAngle, const Vector3<T> &vAxis );

        /**
         * Applies a rotation in the form of quaternion
         */
        void selfRotate( const Quaternion<T> &qRotation );

        /**
         * Echos the vector in an output stream
         */
        void write( ostream &cout ) const;

        /**
         * Reads the vector from an input stream
         */
        void read( istream &cin );

        /**
         * Adds a value to x component
         */
        void addX( T x ) { _xyz[0] += x; }

        /**
         * Adds a value to y component
         */
        void addY( T y ) { _xyz[1] += y; }

        /**
         * Adds a value to z component
         */
        void addZ( T z ) { _xyz[2] += z; }
        
        /**
         * Adds a value to all components
         */
        void add( T amount ) 
        { 
            _xyz[0] += amount; 
            _xyz[1] += amount; 
            _xyz[2] += amount; 
        }

        void add( T x, T y, T z ) 
        {
            _xyz[0] += x; 
            _xyz[1] += y; 
            _xyz[2] += z; 
        }

        void add( T xyz[] ) 
        { 
            _xyz[0] += xyz[0]; 
            _xyz[1] += xyz[1]; 
            _xyz[2] += xyz[2]; 
        }

        void add( const Vector3<T> &vec ) 
        { 
            _xyz[0] += vec.x(); 
            _xyz[1] += vec.y(); 
            _xyz[2] += vec.z(); 
        }

        const Vector3<T> operator+( const Vector3<T> &vec ) const
        { 
            return Vector3<T>( _xyz[0] + vec.x(), _xyz[1] + vec.y(), _xyz[2] + vec.z() );
        }

        const Vector3<T> operator+( T val ) const
        { 
            return Vector3<T>( _xyz[0] + val, _xyz[1] + val, _xyz[2] + val ); 
        }

        Vector3<T>& operator+=( const Vector3<T> &vec ) 
        { 
            add( vec ); 
            return *this;
        }

        Vector3<T>& operator+=( T val ) 
        { 
            add( val );
            return *this; 
        }

        // -- Subtraction
        void subX( T x ) { _xyz[0] -= x; }
        void subY( T y ) { _xyz[1] -= y; }
        void subZ( T z ) { _xyz[2] -= z; }
        
        void sub( T amount ) 
        { 
            _xyz[0] -= amount; 
            _xyz[1] -= amount; 
            _xyz[2] -= amount; 
        }

        void sub( T x, T y, T z ) 
        {
            _xyz[0] -= x; 
            _xyz[1] -= y; 
            _xyz[2] -= z; 
        }

        void sub( T xyz[] ) 
        { 
            _xyz[0] -= xyz[0]; 
            _xyz[1] -= xyz[1]; 
            _xyz[2] -= xyz[2]; 
        }

        void sub( const Vector3<T> &vec ) 
        { 
            _xyz[0] -= vec.x(); 
            _xyz[1] -= vec.y(); 
            _xyz[2] -= vec.z(); 
        }

        const Vector3<T> operator-( const Vector3<T> &vec ) const
        { 
            return Vector3<T>( _xyz[0] - vec.x(), 
                               _xyz[1] - vec.y(), 
                               _xyz[2] - vec.z() ); 
        }

        const Vector3<T> operator-( T val ) const
        { 
            return Vector3<T>( _xyz[0] - val, 
                               _xyz[1] - val, 
                               _xyz[2] - val ); 
        }

        Vector3<T>& operator-=( const Vector3<T> &vec ) 
        { 
            _xyz[0] -= vec.x(); 
            _xyz[1] -= vec.y(); 
            _xyz[2] -= vec.z(); 
            
            return *this; 
        }

        Vector3<T>& operator-=( T val ) 
        { 
            _xyz[0] -= val; 
            _xyz[1] -= val; 
            _xyz[2] -= val;  
            
            return *this; 
        }

        // -- Multiplication
        void mulX( T x ) { _xyz[0] *= x; }
        void mulY( T y ) { _xyz[1] *= y; }
        void mulZ( T z ) { _xyz[2] *= z; }
        
        void mul( T amount ) 
        { 
            _xyz[0] *= amount; 
            _xyz[1] *= amount; 
            _xyz[2] *= amount; 
        }

        void mul( T x, T y, T z ) 
        {
            _xyz[0] *= x; 
            _xyz[1] *= y; 
            _xyz[2] *= z; 
        }

        void mul( T xyz[] ) 
        { 
            _xyz[0] *= xyz[0]; 
            _xyz[1] *= xyz[1]; 
            _xyz[2] *= xyz[2]; 
        }

        void mul( const Vector3<T> &vec ) 
        { 
            _xyz[0] *= vec.x(); 
            _xyz[1] *= vec.y(); 
            _xyz[2] *= vec.z(); 
        }

        const Vector3<T> operator*( const Vector3<T> &vec ) const
        { 
            return Vector3<T>( _xyz[0] * vec.x(), 
                               _xyz[1] * vec.y(), 
                               _xyz[2] * vec.z() ); 
        }

        const Vector3<T> operator*( T val ) const
        { 
            return Vector3<T>( _xyz[0] * val, 
                               _xyz[1] * val, 
                               _xyz[2] * val ); 
        }

        Vector3<T>& operator*=( const Vector3<T> &vec ) 
        { 
            _xyz[0] *= vec.x(); 
            _xyz[1] *= vec.y(); 
            _xyz[2] *= vec.z(); 
            
            return *this; 
        }

        Vector3<T>& operator*=( T val ) 
        { 
            _xyz[0] *= val; 
            _xyz[1] *= val; 
            _xyz[2] *= val;  
            
            return *this; 
        }

        // -- Division
        void divX( T x ) { _xyz[0] /= x; }
        void divY( T y ) { _xyz[1] /= y; }
        void divZ( T z ) { _xyz[2] /= z; }
        
        void div( T amount ) 
        { 
            _xyz[0] /= amount; 
            _xyz[1] /= amount; 
            _xyz[2] /= amount; 
        }

        void div( T x, T y, T z ) 
        {
            _xyz[0] /= x; 
            _xyz[1] /= y; 
            _xyz[2] /= z; 
        }

        void div( T xyz[] ) 
        { 
            _xyz[0] /= xyz[0]; 
            _xyz[1] /= xyz[1]; 
            _xyz[2] /= xyz[2]; 
        }

        void div( const Vector3<T> &vec ) 
        { 
            _xyz[0] /= vec.x(); 
            _xyz[1] /= vec.y(); 
            _xyz[2] /= vec.z(); 
        }

        const Vector3<T> operator/( const Vector3<T> &vec ) const
        { 
            return Vector3<T>( _xyz[0] / vec.x(), 
                               _xyz[1] / vec.y(), 
                               _xyz[2] / vec.z() ); 
        }

        const Vector3<T> operator/( const T val ) const
        { 
            float temp = 1.0f / (float)val;
            return Vector3<T>( _xyz[0] * temp, 
                               _xyz[1] * temp, 
                               _xyz[2] * temp ); 
        }

        Vector3<T>& operator/=( const Vector3<T> &vec ) 
        { 
            _xyz[0] /= vec.x(); 
            _xyz[1] /= vec.y(); 
            _xyz[2] /= vec.z(); 
            
            return *this; 
        }

        Vector3<T>& operator/=( const T val ) 
        { 
            float temp = 1.0f / (float)val;
            _xyz[0] *= temp; 
            _xyz[1] *= temp; 
            _xyz[2] *= temp;  
            
            return *this; 
        }

        /**
         * Predifed common vectors
         */
        static Vector3<T> Down() { 
            return Vector3<T>( Ô(0), T(-1), Ô(0) ); 
        }
        static Vector3<T> Up() { 
            return Vector3<T>( T(0), T(1), T(0) ); 
        }
        static Vector3<T> Left() { 
            return Vector3<T>( T(-1), T(0), T(0) ); 
        }
        static Vector3<T> Right() { 
            return Vector3<T>( T(1), T(0), T(0) ); 
        }
        static Vector3<T> Front() { 
            return Vector3<T>( T(0), T(0), T(-1) ); 
        }
        static Vector3<T> Back() { 
            return Vector3<T>( T(0), T(0), T(1) ); 
        }
        static Vector3<T> Null() { 
            return Vector3<T>( T(0), T(0), T(0) ); 
        }

    }; // end Vector3<T> class

    /** Members functions **/
    template<typename T>
    void Vector3<T>::normalize()
    {
        // get vector length
        float len = length();
        
        // if lenth already unit then return :)
        //if( fcmp( len, 1.0f ) ) return;

        // if length is zero, then the components are all zero
        if( len < 1e-6 )
        {
            _xyz[0] = (T)0;
            _xyz[1] = (T)0;
            _xyz[2] = (T)0;
            return;
        }
        
        // choose to divide once and multiply thrice
        // instead of just dividing three times
        len = 1.0f / len;
        _xyz[0] *= len;
        _xyz[1] *= len;
        _xyz[2] *= len;

    } // end normalize()

    template<typename T>
    void Vector3<T>::normalize( Vector3<T> &vec ) const
    {
        // get vector length
        float len = (float)sqrt( squaredLength() );
        
        // if lenth already unit then return the current values
        if( fcmp( len, 1.0f ) )
        {
            vec.x( _xyz[0] );
            vec.y( _xyz[1] );
            vec.z( _xyz[2] );
            return;
        }

        // if length is zero, then the components are all zero
        if( len < FZERO_E )
        {
            vec.x((T)0);
            vec.y((T)0);
            vec.z((T)0);
            return;
        }

        // choose to divide once and multiply thrice
        // instead of just dividing three times
        len = 1.0f / len;
        vec.x( _xyz[0] * len );
        vec.y( _xyz[1] * len );
        vec.z( _xyz[2] * len );

    } // end normalizeTo()

    /**
     * Rotates the vector by angle radians about a given axis
     */
    template<typename T>
    Vector3<T> Vector3<T>::rotate( const float fAngle, const Vector3<T> &vAxis ) const
    {   
        // Create rotation quaternion
        Quaternion<T> qRotation( fAngle, vAxis );
        qRotation.toRotation();

        return rotate( qRotation );

    } // end rotate()

    template<typename T>
    Vector3<T> Vector3<T>::rotate( const Quaternion<T> &qRotation ) const
    {   
        // Create quaternion after the vector data
        Quaternion<T> qVec( 0.0f, _xyz );

        // Apply rotation
        Quaternion<T> qResult = qRotation * qVec * (~qRotation);

        // Return rotated vector components
        return qResult.xyz();

    } // end rotate()

    template<typename T>
    void Vector3<T>::selfRotate( const float fAngle, const Vector3<T> &vAxis )
    {   
        // Create rotation quaternion
        Quaternion<T> qRotation( fAngle, vAxis );
        qRotation.toRotation();

        selfRotate( qRotation );

    } // end selfRotate()

    template<typename T>
    void Vector3<T>::selfRotate( const Quaternion<T> &qRotation )
    {   
        // Create quaternion after the vector data
        Quaternion<T> qVec( 0.0f, _xyz );

        // Apply rotation
        Quaternion<T> qResult = qRotation * qVec * (~qRotation);

        // Replace rotated vector with self
        xyz( qResult.xyz() );

    } // end selfRotate()

    template<typename T>
    void Vector3<T>::write( ostream &cout ) const
    {
        cout << "( " << _xyz[0] << ", "
                     << _xyz[1] << ", "
                     << _xyz[2] << " )";
    }

    template<typename T>
    void Vector3<T>::read( istream &cin )
    {
        char ch[10];
        cin >> ch >> _xyz[0]
            >> ch >> _xyz[1]
            >> ch >> _xyz[2] >> ch;
    }

    /** Cover consistency and support for mixed-mode arithmetic **/
    /**
     * Multiplies a vector with a value and returns the result vector
     */
    template<typename T>
    inline Vector3<T> operator*( const Vector3<T> &vec1, T val )
    { 
        return Vector3<T>( vec1.x() * val, 
                           vec1.y() * val,
                           vec1.z() * val );
    }

    template<typename T>
    inline Vector3<T> operator*( T val, const Vector3<T> &vec1 )
    { 
        return ( vec1 * val );
    }

    /**
     * Subtracts two vectors and returns the result vector
     */
    template<typename T>
    inline Vector3<T> operator-( const Vector3<T> &vec1, 
                                 const Vector3<T> &vec2 ) 
    { 
        return Vector3<T>( vec1.x() - vec2.x(), 
                           vec1.y() - vec2.y(),
                           vec1.z() - vec2.z() );
    }

    /**
     * Echos the vector in an output stream returning the stream
     * to support chainability
     */
    template<typename T>
    ostream& operator<<( ostream &cout, const Vector3<T> &vec )
    {
        vec.write( cout );
        return cout;
    }

    /**
     * Reads a vector from an input stream returning the stream
     * to support chainability
     */
    template<typename T>
    istream& operator>>( istream &cin, Vector3<T> &vec )
    {
        vec.read( cin );
        return cin;
    }

    //typedef Vector3<int>        Vector3i;
    typedef Vector3<float>      Vector3f;
    //typedef Vector3<double>     Vector3d;

} // end of tlib namespace