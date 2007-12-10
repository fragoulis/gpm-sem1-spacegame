#pragma once
#include <iostream>
#include "math.h"
using std::ostream;
using std::istream;

namespace tlib 
{
    class Vector3f
    {
    private:
        // The vector's components in array format
        float _xyz[3];

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
        Vector3f() {}
        Vector3f( const float xyz[] ) { this->xyz( xyz ); }
        Vector3f( float x, float y, float z ) { xyz( x, y, z ); }
        
        /**
         * Sets all components to zero
         */
        void reset() { x(0.0f); y(0.0f); z(0.0f); }

        /**
         * Component accessors
         */
        const float x() const { return _xyz[0]; }
        const float y() const { return _xyz[1]; }
        const float z() const { return _xyz[2]; }

        /**
         * Returns all components
         */
        const float* xyz() const { return _xyz; }

        /** 
         * Component mutators [single/multiple]
         */
        void x( float x ) { _xyz[0] = x; }
        void y( float y ) { _xyz[1] = y; }
        void z( float z ) { _xyz[2] = z; }

        void xyz( float x, float y, float z ) { 
            this->x(x); this->y(y); this->z(z);
        }

        void xyz( const float xyz[] ) { 
            x(xyz[0]); y(xyz[1]); z(xyz[2]);
        }
        
        void xyz( const Vector3f &vec ) { 
            x(vec.x()); y(vec.y()); z(vec.z());
        }

        /**
         * Returns the length of the vector
         */
        float length() const { 
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
        float dot( const Vector3f &vec ) const {
            return ( x()*vec.x() + y()* vec.y() + z()*vec.z() );
        }

        /**
         * Calculates the cross product
         */
        Vector3f cross( const Vector3f &vec ) const {
            return Vector3f( y() * vec.z() - z() * vec.y(),
                               z() * vec.x() - x() * vec.z(),
                               x() * vec.y() - y() * vec.x() );
        }

        /**
         * Normalizes the vector
         */
        inline void normalize();

        /** 
         * Saves the normalized components to the given vector
         */
        inline void normalize( Vector3f &vec ) const;

        /**
         * Rotates the vector by angle radians about a given axis
         * and returns the resulting rotated vector
         */
        Vector3f rotate( const float fAngle, const Vector3f &vAxis ) const;

        /**
         * Applies a rotation in the form of quaternion
         * and returns the resulting rotated vector
         */
        Vector3f rotate( const Quatf &qRotation ) const;

        /**
         * Rotates the vector by angle radians about a given axis
         */
        void selfRotate( const float fAngle, const Vector3f &vAxis );

        /**
         * Applies a rotation in the form of quaternion
         */
        void selfRotate( const Quatf &qRotation );

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
        void addX( float x ) { _xyz[0] += x; }

        /**
         * Adds a value to y component
         */
        void addY( float y ) { _xyz[1] += y; }

        /**
         * Adds a value to z component
         */
        void addZ( float z ) { _xyz[2] += z; }
        
        /**
         * Adds a value to all components
         */
        void add( float amount ) 
        { 
            _xyz[0] += amount; 
            _xyz[1] += amount; 
            _xyz[2] += amount; 
        }

        void add( float x, float y, float z ) 
        {
            _xyz[0] += x; 
            _xyz[1] += y; 
            _xyz[2] += z; 
        }

        void add( float xyz[] ) 
        { 
            _xyz[0] += xyz[0]; 
            _xyz[1] += xyz[1]; 
            _xyz[2] += xyz[2]; 
        }

        void add( const Vector3f &vec ) 
        { 
            _xyz[0] += vec.x(); 
            _xyz[1] += vec.y(); 
            _xyz[2] += vec.z(); 
        }

        const Vector3f operator+( const Vector3f &vec ) const
        { 
            return Vector3f( _xyz[0] + vec.x(), _xyz[1] + vec.y(), _xyz[2] + vec.z() );
        }

        const Vector3f operator+( float val ) const
        { 
            return Vector3f( _xyz[0] + val, _xyz[1] + val, _xyz[2] + val ); 
        }

        Vector3f& operator+=( const Vector3f &vec ) 
        { 
            add( vec ); 
            return *this;
        }

        Vector3f& operator+=( float val ) 
        { 
            add( val );
            return *this; 
        }

        // -- Subtraction
        void subX( float x ) { _xyz[0] -= x; }
        void subY( float y ) { _xyz[1] -= y; }
        void subZ( float z ) { _xyz[2] -= z; }
        
        void sub( float amount ) 
        { 
            _xyz[0] -= amount; 
            _xyz[1] -= amount; 
            _xyz[2] -= amount; 
        }

        void sub( float x, float y, float z ) 
        {
            _xyz[0] -= x; 
            _xyz[1] -= y; 
            _xyz[2] -= z; 
        }

        void sub( float xyz[] ) 
        { 
            _xyz[0] -= xyz[0]; 
            _xyz[1] -= xyz[1]; 
            _xyz[2] -= xyz[2]; 
        }

        void sub( const Vector3f &vec ) 
        { 
            _xyz[0] -= vec.x(); 
            _xyz[1] -= vec.y(); 
            _xyz[2] -= vec.z(); 
        }

        const Vector3f operator-( const Vector3f &vec ) const
        { 
            return Vector3f( _xyz[0] - vec.x(), 
                               _xyz[1] - vec.y(), 
                               _xyz[2] - vec.z() ); 
        }

        const Vector3f operator-( float val ) const
        { 
            return Vector3f( _xyz[0] - val, 
                               _xyz[1] - val, 
                               _xyz[2] - val ); 
        }

        Vector3f& operator-=( const Vector3f &vec ) 
        { 
            _xyz[0] -= vec.x(); 
            _xyz[1] -= vec.y(); 
            _xyz[2] -= vec.z(); 
            
            return *this; 
        }

        Vector3f& operator-=( float val ) 
        { 
            _xyz[0] -= val; 
            _xyz[1] -= val; 
            _xyz[2] -= val;  
            
            return *this; 
        }

        // -- Multiplication
        void mulX( float x ) { _xyz[0] *= x; }
        void mulY( float y ) { _xyz[1] *= y; }
        void mulZ( float z ) { _xyz[2] *= z; }
        
        void mul( float amount ) 
        { 
            _xyz[0] *= amount; 
            _xyz[1] *= amount; 
            _xyz[2] *= amount; 
        }

        void mul( float x, float y, float z ) 
        {
            _xyz[0] *= x; 
            _xyz[1] *= y; 
            _xyz[2] *= z; 
        }

        void mul( float xyz[] ) 
        { 
            _xyz[0] *= xyz[0]; 
            _xyz[1] *= xyz[1]; 
            _xyz[2] *= xyz[2]; 
        }

        void mul( const Vector3f &vec ) 
        { 
            _xyz[0] *= vec.x(); 
            _xyz[1] *= vec.y(); 
            _xyz[2] *= vec.z(); 
        }

        const Vector3f operator*( const Vector3f &vec ) const
        { 
            return Vector3f( _xyz[0] * vec.x(), 
                               _xyz[1] * vec.y(), 
                               _xyz[2] * vec.z() ); 
        }

        const Vector3f operator*( float val ) const
        { 
            return Vector3f( _xyz[0] * val, 
                               _xyz[1] * val, 
                               _xyz[2] * val ); 
        }

        Vector3f& operator*=( const Vector3f &vec ) 
        { 
            _xyz[0] *= vec.x(); 
            _xyz[1] *= vec.y(); 
            _xyz[2] *= vec.z(); 
            
            return *this; 
        }

        Vector3f& operator*=( float val ) 
        { 
            _xyz[0] *= val; 
            _xyz[1] *= val; 
            _xyz[2] *= val;  
            
            return *this; 
        }

        // -- Division
        void divX( float x ) { _xyz[0] /= x; }
        void divY( float y ) { _xyz[1] /= y; }
        void divZ( float z ) { _xyz[2] /= z; }
        
        void div( float amount ) 
        { 
            _xyz[0] /= amount; 
            _xyz[1] /= amount; 
            _xyz[2] /= amount; 
        }

        void div( float x, float y, float z ) 
        {
            _xyz[0] /= x; 
            _xyz[1] /= y; 
            _xyz[2] /= z; 
        }

        void div( float xyz[] ) 
        { 
            _xyz[0] /= xyz[0]; 
            _xyz[1] /= xyz[1]; 
            _xyz[2] /= xyz[2]; 
        }

        void div( const Vector3f &vec ) 
        { 
            _xyz[0] /= vec.x(); 
            _xyz[1] /= vec.y(); 
            _xyz[2] /= vec.z(); 
        }

        const Vector3f operator/( const Vector3f &vec ) const
        { 
            return Vector3f( _xyz[0] / vec.x(), 
                               _xyz[1] / vec.y(), 
                               _xyz[2] / vec.z() ); 
        }

        const Vector3f operator/( const float val ) const
        { 
            float temp = 1.0f / (float)val;
            return Vector3f( _xyz[0] * temp, 
                               _xyz[1] * temp, 
                               _xyz[2] * temp ); 
        }

        Vector3f& operator/=( const Vector3f &vec ) 
        { 
            _xyz[0] /= vec.x(); 
            _xyz[1] /= vec.y(); 
            _xyz[2] /= vec.z(); 
            
            return *this; 
        }

        Vector3f& operator/=( const float val ) 
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
        static Vector3f Down() { 
            return Vector3f( 0.0f, -1.0f, 0.0f ); 
        }
        static Vector3f Up() { 
            return Vector3f( 0.0f, 1.0f, 0.0f ); 
        }
        static Vector3f Left() { 
            return Vector3f( -1.0f, 0.0f, 0.0f ); 
        }
        static Vector3f Right() { 
            return Vector3f( 1.0f, 0.0f, 0.0f ); 
        }
        static Vector3f Front() { 
            return Vector3f( 0.0f, 0.0f, -1.0f ); 
        }
        static Vector3f Back() { 
            return Vector3f( 0.0f, 0.0f, 1.0f ); 
        }
        static Vector3f Null() { 
            return Vector3f( 0.0f, 0.0f, 0.0f ); 
        }

    }; // end Vector3f class

    /** Members functions **/
    void Vector3f::normalize()
    {
        // get vector length
        float len = length();
        
        // if lenth already unit then return :)
        //if( fcmp( len, 1.0f ) ) return;

        // if length is zero, then the components are all zero
        if( len < 1e-6 )
        {
            _xyz[0] = 0.0f;
            _xyz[1] = 0.0f;
            _xyz[2] = 0.0f;
            return;
        }
        
        // choose to divide once and multiply thrice
        // instead of just dividing three times
        len = 1.0f / len;
        _xyz[0] *= len;
        _xyz[1] *= len;
        _xyz[2] *= len;

    } // end normalize()

    void Vector3f::normalize( Vector3f &vec ) const
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
            vec.x(0.0f);
            vec.y(0.0f);
            vec.z(0.0f);
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
    Vector3f Vector3f::rotate( const float fAngle, const Vector3f &vAxis ) const
    {   
        // Create rotation quaternion
        Quatf qRotation( fAngle, vAxis );
        qRotation.toRotation();

        return rotate( qRotation );

    } // end rotate()

    Vector3f Vector3f::rotate( const Quatf &qRotation ) const
    {   
        // Create quaternion after the vector data
        Quatf qVec( 0.0f, _xyz );

        // Apply rotation
        Quatf qResult = qRotation * qVec * (~qRotation);

        // Return rotated vector components
        return qResult.xyz();

    } // end rotate()

    void Vector3f::selfRotate( const float fAngle, const Vector3f &vAxis )
    {   
        // Create rotation quaternion
        Quatf qRotation( fAngle, vAxis );
        qRotation.toRotation();

        selfRotate( qRotation );

    } // end selfRotate()

    void Vector3f::selfRotate( const Quatf &qRotation )
    {   
        // Create quaternion after the vector data
        Quatf qVec( 0.0f, _xyz );

        // Apply rotation
        Quatf qResult = qRotation * qVec * (~qRotation);

        // Replace rotated vector with self
        xyz( qResult.xyz() );

    } // end selfRotate()

    void Vector3f::write( ostream &cout ) const
    {
        cout << "( " << _xyz[0] << ", "
                     << _xyz[1] << ", "
                     << _xyz[2] << " )";
    }

    void Vector3f::read( istream &cin )
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
    inline Vector3f operator*( const Vector3f &vec1, float val )
    { 
        return Vector3f( vec1.x() * val, 
                           vec1.y() * val,
                           vec1.z() * val );
    }

    inline Vector3f operator*( float val, const Vector3f &vec1 )
    { 
        return ( vec1 * val );
    }

    /**
     * Subtracts two vectors and returns the result vector
     */
    inline Vector3f operator-( const Vector3f &vec1, 
                                 const Vector3f &vec2 ) 
    { 
        return Vector3f( vec1.x() - vec2.x(), 
                           vec1.y() - vec2.y(),
                           vec1.z() - vec2.z() );
    }

    /**
     * Echos the vector in an output stream returning the stream
     * to support chainability
     */
    ostream& operator<<( ostream &cout, const Vector3f &vec )
    {
        vec.write( cout );
        return cout;
    }

    /**
     * Reads a vector from an input stream returning the stream
     * to support chainability
     */
    istream& operator>>( istream &cin, Vector3f &vec )
    {
        vec.read( cin );
        return cin;
    }

} // end of tlib namespace