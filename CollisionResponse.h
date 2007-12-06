#pragma once
#include "Component.h"
#include "Vector3.h"

namespace tlib
{

    class IOCCollisionResponse : public IComponent
    {
    private:
        // The collision direction
        Vector3f m_vCollDir;

        // The collision point
        Vector3f m_vCollPoint;

    public:
        /**
         * Destructor
         */
        virtual ~IOCCollisionResponse(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("collisionresponse"); 
        }

        virtual const string componentID() const {
            return string("collisionresponse"); 
        }

        /**
         * Collision direction getter/setter
         */
        const Vector3f& getCollDir() const {
            return m_vCollDir;
        }
        void setCollDir( const Vector3f &vCollDir ) {
            m_vCollDir = vCollDir;
        }
    
        /**
         * Collision point getter/setter
         */
        const Vector3f& getCollPoint() const {
            return m_vCollPoint;
        }
        void setCollPoint( const Vector3f &vCollPoint ) {
            m_vCollPoint = vCollPoint;
        }

        /**
         * Responds to the collision
         * The parameter is the collision vector [optional]
         */
        virtual void respond() const;

    };

} // end of namespace tlib