#pragma once
#include "Visual.h"

namespace tlib
{

    class OCVisualDisplayList : public IOCVisual
    {
    private:
        // The diplay list's ID
        unsigned int m_uiListId;

    public:
        /**
         * Destructor
         */
        virtual ~OCVisualDisplayList();

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("displaylist"); 
        }

        /**
         * Renders the model
         */
        void render() const;

        /**
         * Generates the display list
         */
        void generate();
        
        /**
         * Virtual function that will build the object most likely using
         * immediate mode
         */
        virtual void buildObject() const = 0;

    }; // end of OCDisplayList class

} // end of namespace tlib