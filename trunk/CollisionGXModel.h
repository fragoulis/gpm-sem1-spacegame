#pragma once
#include "CollisionDynamicBBox.h"

namespace tlib 
{
    class OCCollisionGXModel : public OCCollisionDynamicBBox
    {
    public:
        /**
         * Constructor
         */
        OCCollisionGXModel();

        /**
         * Destructor
         */
        virtual ~OCCollisionGXModel(){}
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("collisiongxmodel"); 
        }

        /**
         * Sets the bounding box for the model
         */
        void initBBox();

    }; // end of OCCollisionGXModel class

} // end of namespace tlib