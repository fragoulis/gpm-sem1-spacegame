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
         * Applies the texture to an object
         */
        virtual void apply() const = 0;

        /**
         * Resets the texture's state
         */
        virtual void reset() const = 0;

        /**
         * Returns the number of textures this component holds
         * The number by default is one, but is overrider in 
         * some components.
         */
        virtual unsigned int getNumOfTextures() const {
            return 1;
        }
    };

} // end of namespace tlib