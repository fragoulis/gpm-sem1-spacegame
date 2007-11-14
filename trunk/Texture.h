#pragma once
#include "component.h"

namespace tlib
{

    class IOCTexture : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCTexture(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("texture"); 
        }

        /**
         * Applies the presentation to an object
         */
        virtual void apply() const = 0;
    };

} // end of namespace tlib