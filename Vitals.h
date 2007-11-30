#pragma once
#include "Component.h"

namespace tlib
{

    class IOCVitals : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCVitals(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("vitals"); 
        }

        /**
         * Updates and checks the vitals of an object
         */
        virtual bool update() = 0;

        /**
         * This is called on an object's death
         */
        virtual void onKill() = 0;

        /**
         * This is called on an object's revival
         */
        virtual void onRevive() = 0;

    };

} // end of namespace tlib