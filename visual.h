
#pragma once
#include "component.h"

namespace tlib 
{

    class IOCVisual : public IComponent
    {
    protected:
        // The dummy opengl rotation matrix
        mutable float m_fRotMatrix[16];

    public:
        /**
         * Destructor
         */
        virtual ~IOCVisual(){}
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("visual"); 
        }

        /**
         * Virtual method that will render the object
         */
        virtual void render() const = 0;

    }; // end of IOCVisual class

} // end of namespace tlib