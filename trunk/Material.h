#pragma once
#include "component.h"

namespace tlib
{

    class IOCMaterial : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCMaterial(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("material"); 
        }

        /**
         * Applies the presentation to an object
         */
        virtual void apply() const = 0;
    };

} // end of namespace tlib