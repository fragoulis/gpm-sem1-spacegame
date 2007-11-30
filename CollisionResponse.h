#pragma once
#include "Component.h"
#include "Vector3.h"

namespace tlib
{

    class IOCCollisionResponse : public IComponent
    {
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
         * Responds to the collision
         * The parameter is the collision vector [optional]
         */
        virtual void respond( const Vector3f &vCollVector ) const;

    };

} // end of namespace tlib