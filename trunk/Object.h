
#pragma once

#include <map>
#include "component.h"
#include "vector3.h"
#include "quaternion.h"

using std::map;

namespace tlib
{
    // Typedef the type of associative array we are going
    // to be using. This let's us map a string identifier 
    // to each component.
    typedef map< string, IComponent* > component_array;

    class Object
    {
    protected:
        // The object's position
        Vector3f m_vPos;

        // The object's direction
        Quatf m_qDir;

        // The list of components
        component_array m_vComponents;

        // Flags whether an object should continue to get 
        // rendered and updated
        bool m_bActive;

    public:
        /**
         * Constructor
         */
        Object(): m_bActive(true) {}

        /**
         * Destructor
         */
        virtual ~Object()
        {
             clearComponentList();           
        }

        /**
         * Frees all memory that the components occupy
         */
        void clearComponentList()
        {
            component_array::iterator iter;
            for( iter = m_vComponents.begin(); 
                 iter != m_vComponents.end(); 
                 iter++ )
            {
                delete iter->second;
                iter->second = 0;
            }
        }

        /**
         * Pushes a component to the component list
         */
        IComponent* setComponent( IComponent *comp ) 
        { 
            // Get the component that MAY already exists of the
            // same interface
            IComponent *oldComp = m_vComponents[ comp->familyID() ];

            // set the owner of this component to be this
            comp->setOwner( this );

            // push the component to the list
            m_vComponents[ comp->familyID() ] = comp;

            return oldComp;
        }

        /**
         * Returns a component by its string identifier (ID)
         */
        IComponent* getComponent( const string &id )
        { 
            return m_vComponents[id]; 
        }

        /**
         * Returns the position
         */
        const Vector3f& getPos() const { return m_vPos; }

        /**
         * Sets the position
         */
        void setPos( const Vector3f& vec ) { m_vPos = vec; }

        /**
         * Returns the direction
         */
        const Quatf& getDir() const { return m_qDir; }

        /**
         * Sets the direction
         */
        void setDir( const Quatf& quat ) { m_qDir = quat; }

        /**
         * Deactivates the object
         */
        void deactivate() { m_bActive = false; }

        /** 
         * Returs the active flag
         */
        bool isActive() const { return m_bActive; }

    }; // end of Object class

} // end of tlib namespace