
#pragma once
#include "component.h"

namespace tlib
{

    class IOCController : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCController(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("controller"); 
        }

    }; // end of IOCController class

} // end of namespace tlib
