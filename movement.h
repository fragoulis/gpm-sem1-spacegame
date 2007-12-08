#pragma once
#include "Component.h"
#include "Vector3.h"

namespace tlib 
{

    class IOCMovement : public IComponent
    {
    protected:
        // The direction vector
        // This vector determines to which direction the object will move
        Vector3f m_vDir;

    public:
        /**
         * Destructor
         */
        virtual ~IOCMovement(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("movement"); 
        }

        /**
         * Updates the object's position, velocity etc
         */
        virtual void update() = 0;

        /**
         * Returns the direction vector
         */
        const Vector3f& getDir() const { return m_vDir; }
        Vector3f& getDir() { return m_vDir; }
        
        /**
         * Sets the direction vector
         */
        void setDir( const Vector3f& vDir ) { m_vDir = vDir; }

    }; // end of IOCMovement class

} // end of namespace tlib