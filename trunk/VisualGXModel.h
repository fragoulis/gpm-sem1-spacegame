
#pragma once

#include "gx/model.h"
#include "visual.h"

using gxbase::Model;

namespace tlib
{

    class OCGXModel : public IOCVisual
    {
    private:
        // Our gxbase model
        Model m_Model;

    public:
        /**
         * Constructors
         */
        OCGXModel();
        OCGXModel( const char *filename );
        OCGXModel( const char *filename, float scale );

        /**
         * Destructors
         */
        virtual ~OCGXModel()
        {
            //m_Model.FreeGL();
            m_Model.Free();
        }

        /**
         * Returns the model
         */
        const Model& getGXModel() const { 
            return m_Model; 
        }
        
        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("gxmodel"); 
        }

        /**
         * Renders the model
         */
        virtual void render() const;

        /**
         * Loads the model with GXModel
         */
        bool load( const char *filename, float scale );

    }; // end of OCGXModel class

} // end of namespace tlib