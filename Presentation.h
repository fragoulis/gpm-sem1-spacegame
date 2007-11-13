
#pragma once
#include "component.h"

namespace tlib
{

    class IOCPresentation : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCPresentation(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("presentation"); 
        }

        /**
         * Applies the presentation to an object
         */
        virtual void apply() const = 0;
    };

} // end of namespace tlib