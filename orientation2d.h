
#pragma once

#include "orientation.h"

namespace tlib 
{

    class OCOrientation2D : public IOCOrientation
    {
    protected:
        // The viewing vector
        Vector3f m_vView;

        // The up vector
        Vector3f m_vUp;

    public:
        /**
         * Destructor
         */
        virtual ~OCOrientation2D(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("orientation2d"); 
        }

        /**
         * Returns the view vector
         */
        const Vector3f& getView() const { return m_vView; }
        Vector3f& getView() { return m_vView; }

        /**
         * Sets the view vector
         */
        void setView( const Vector3f& vec ) { m_vView = vec; }

        /**
         * Returns the up vector
         */
        const Vector3f& getUp() const { return m_vUp; }
        Vector3f& getUp() { return m_vUp; }

        /**
         * Sets the up vector
         */
        void setUp( const Vector3f& vec ) { m_vUp = vec; }

        /**
         * Rotates the view about an arbitrary axis by a given angle in degrees
         */
        virtual void rotate( const float fAngle, const Vector3f &vAxis );

        /**
         * Rotates the view about the object's right vector
         */
        virtual void pitch( const float fAngle );

        /**
         * Rotates the view about the object's up vector
         */
        virtual void yaw( const float fAngle );

        /**
         * Rotates the view about the object's view vector
         */
        virtual void roll( const float fAngle );

        /**
         * Changes the position of the camera
         */
        virtual void positionAt( const Vector3f &vNewPos );

        /**
         * Moves the target of the camera
         */
        virtual void lookAt( const Vector3f &vNewPos );

        /**
         * Moves the camera and sets a new target
         */
        virtual void positionAtAndLookAt( const Vector3f &vNewPos, 
                                          const Vector3f &vLookAt );

    protected:
        /**
         * Calculates the angle between the new viewing vector and the old one
         * and rotates the up vector by that angle along the axis defined by
         * the cross product of the new and old viewing vectors
         */
        void _updateUpVector( Vector3f &vNewView );

    }; // end of OCOrientation2D class

} // end of namespace tlib