#pragma once
#include "orientation3d.h"
#include "object.h"

namespace tlib
{

    class OCQuatRotation : public OCOrientation3D
    {
    private:
        // The objects rotations
        Quatf m_qRot;
        Quatf m_qPrevRot;
        Quatf m_qPitch;
        Quatf m_qYaw;
        Quatf m_qRoll;

    public:
        virtual ~OCQuatRotation() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("quatrotation"); 
        }

        /**
         * Returns the rotation quaternion
         */
        const Quatf& getRot() const { return m_qRot; }
        Quatf& getRot() { return m_qRot; }

        /**
         * Returns the previous rotation quaternion
         */
        const Quatf& getPrevRot() const { return m_qPrevRot; }
        Quatf& getPrevRot() { return m_qPrevRot; }

        /**
         * Returns the previous pitch
         */
        const Quatf& getPitch() const { return m_qPitch; }
        Quatf& getPitch() { return m_qPitch; }

        /**
         * Returns the previous yaw
         */
        const Quatf& getYaw() const { return m_qYaw; }
        Quatf& getYaw() { return m_qYaw; }

        /**
         * Returns the previous roll
         */
        const Quatf& getRoll() const { return m_qRoll; }
        Quatf& getRoll() { return m_qRoll; }

        /**
         * Sets the rotation quaternion
         */
        void setRot( const Quatf& qRot ) { m_qRot = qRot; }

        /**
         * Multiplies the current rotation with another quaternion
         * and also returns the resulting rotation
         */
        const Quatf& addRotation( const Quatf& qNewRot );

        /**
         * Multiplies the current rotation with another quaternion
         * which creates internally and also returns the resulting rotation
         */
        const Quatf& addRotation( const float fAngle, const Vector3f& vAxis );

        /**
         * 
         */
        const Quatf& addPitch( const float fAngle );
        const Quatf& addYaw( const float fAngle );
        const Quatf& addRoll( const float fAngle );
        void resetAngles();

        /**
         * Updates the object's orientation
         */
        void update();
        void updateView( const Quatf& qRot );
        void updateUp( const Quatf& qRot );

    }; // end of OCQuatRotation class

} // end of namespace tlib