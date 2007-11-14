#pragma once
#include "component.h"
#include "Object.h"

namespace tlib 
{
    class Object;

    class IOCCollision : public IComponent
    {
    protected:
        // This variable is set in the derived classes
        // Indicates what kind of bounding geom (sphere,box)
        // the object has
        int m_iType;

        // This flag tells us whether the collision is turned on for
        // the owner object
        bool m_bIsActive;

    public:
        /**
         * Constructor
         */
        IOCCollision();

        /**
         * Destructor
         */
        virtual ~IOCCollision(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("collision"); 
        }

        /**
         * Returns the type of the bounding box
         */
        int getType() const { return m_iType; }

        /**
         * Activates/Deactivates the component
         */
        void activate() { m_bIsActive = true; }
        void deactivate() { m_bIsActive = false; }
        bool isActive() const { return m_bIsActive; }

        /**
         * Checks for collision between this object and another
         * Returns true if collision is detected and sets a vector
         * with the collision direction
         */
        virtual bool check( Object *oObj, Vector3f& ) const;
        
        /**
         * Checks for collision between this object and another
         * Returns true if collision is detected
         */
        virtual bool check( Object *oObj ) const;

        /**
         * This function is called after a collision has been detected
         */
        //virtual void response() = 0;

        // Values for member type
        enum BType { BBox = 1, BSphere, BPlane };

    protected:
        /**
         * Checks the collision of a box with a box
         * Returns true if collision is detected
         */
        virtual bool checkBoxWithBox( const Vector3f&, const Vector3f& ) const;
        
        /**
         * Checks the collision of a box with a sphere
         * Returns true if collision is detected
         */
        virtual bool checkBoxWithSphere( const Vector3f&, int ) const;

        /**
         * Checks the collision of a sphere with a sphere
         * Returns true if collision is detected
         */
        virtual bool checkSphereWithSphere( const Vector3f&, int  ) const;

        /**
         * Checks the collision of a box with a box
         * Returns true if collision is detected and sets a vector
         * with the collision direction
         */
        virtual bool checkBoxWithBox( const Vector3f&, const Vector3f&, 
                                      Vector3f& ) const;
        
        /**
         * Checks the collision of a box with a sphere
         * Returns true if collision is detected and sets a vector
         * with the collision direction
         */
        virtual bool checkBoxWithSphere( const Vector3f&, int, 
                                         Vector3f&  ) const;

        /**
         * Checks the collision of a sphere with a sphere
         * Returns true if collision is detected and sets a vector
         * with the collision direction
         */
        virtual bool checkSphereWithSphere( const Vector3f&, int, 
                                            Vector3f&  ) const;

    }; // end of IOCCollision class

} // end of namespace tlib