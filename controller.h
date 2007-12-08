
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

		/**
         * Executes after capturing user's input user input
         */
        virtual void update() = 0;

    }; // end of IOCController class

} // end of namespace tlib
