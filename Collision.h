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

    public:
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