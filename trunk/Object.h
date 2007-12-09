#pragma once
#include "component.h"
#include "vector3.h"
#include "quaternion.h"
#include <map>
using std::map;

namespace tlib
{
    // Typedef the type of associative array we are going
    // to be using. This let's us map a string identifier 
    // to each component.
    typedef map< string, IComponent* > component_array;

    class Object
    {
    public:
        // A enumeration to help as identify which object is what
        enum ObjectType {
            DEFAULT = 0,
            SPACESHIP, 
            SPACESHIP_SHIELD,
            TURRET,
            BARRIER, // This could alternatively be the barrier's separately
            OUTLET,
            REACTOR,
            LIGHT,
            SPACECUBE,
            NUM_OF_OBJECTS
        };

    private:
        // The object's position
        Vector3f m_vPos;

        // The object's direction
        Quatf m_qDir;

        // Flags whether an object should continue to get 
        // rendered and updated
        bool m_bActive;

        // The type of the object
        ObjectType m_iType;

        // The list of components
        mutable component_array m_vComponents;

    public:
        /**
         * Constructor
         */
        Object();

        /**
         * Destructor
         */
        virtual ~Object();

        /**
         * Frees all memory that the components occupy
         */
        void clearComponentList();

        /**
         * Pushes a component to the component list
         */
        IComponent* setComponent( IComponent *comp );

        /**
         * Returns a component by its string identifier (ID)
         * const and non-const
         */
        const IComponent* getComponent( const string &id ) const { 
            return m_vComponents[id]; 
        }
        IComponent* getComponent( const string &id ) { 
            return m_vComponents[id]; 
        }

        /**
         * Returns the position
         */
        const Vector3f& getPos() const { return m_vPos; }
        Vector3f& getPos() { return m_vPos; }

        /**
         * Sets the position
         */
        void setPos( const Vector3f& vec ) { m_vPos = vec; }

        /**
         * Returns the direction
         */
        const Quatf& getDir() const { return m_qDir; }
        Quatf& getDir() { return m_qDir; }

        /**
         * Sets the direction
         */
        void setDir( const Quatf& quat ) { m_qDir = quat; }

        /**
         * Deactivates the object
         */
        void deactivate() { m_bActive = false; }

        /**
         * Activates the object
         */
        void activate() { m_bActive = true; }

        /** 
         * Returs the active flag
         */
        bool isActive() const { return m_bActive; }

        /**
         * Getter/Setter for the type of the object
         */
        int getType() const { return m_iType; }
        void setType( ObjectType iType ) {
            m_iType = iType;
        }

    }; // end of Object class

} // end of tlib namespace