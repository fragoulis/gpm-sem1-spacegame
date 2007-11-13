
#pragma once

#include "component.h"
#include "vector3.h"

namespace tlib 
{

    class IOCOrientation : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCOrientation(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("orientation"); 
        }

        /**
         * These functions define how the object is rotated
         * Note: Angles are always in radians
         */
        virtual void rotate( const float, const Vector3f& ) = 0;
        virtual void pitch( const float ) = 0;
        virtual void yaw( const float ) = 0;
        virtual void roll( const float ) = 0;
        virtual void lookAt( const Vector3f& ) = 0;
        virtual void positionAt( const Vector3f& ) = 0;
        virtual void positionAtAndLookAt( const Vector3f&, 
                                          const Vector3f& ) = 0;

    }; // end of IOCOrientation class

} // end of namespace tlib