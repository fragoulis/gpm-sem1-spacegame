#pragma once
#include "component.h"

namespace tlib
{

    class IOCAI : public IComponent
    {
    public:
        /**
         * Destructor
         */
        virtual ~IOCAI(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("ai"); 
        }

        /**
         * Updates the object's transformation and acts
         * on it according to some predefined conditions
         */
        virtual void update( Object* ) = 0;

    }; // end of IOCAI class

} // end of namespace tlib