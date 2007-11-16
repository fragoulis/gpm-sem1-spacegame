
#pragma once

#include <string>
#include "render.h"
#include "presentation.h"

using std::string;

namespace tlib
{

    class OCGluQuadric : public IOCRender
    {
    protected:
       GLUquadric *m_GluObj;

    public:
        /**
         * Constructor
         */
        OCGluQuadric(): m_GluObj(NULL) {}

        /**
         * Destructor
         */
        virtual ~OCGluQuadric()
        {
            std::cout << "Quadric destructor" << std::endl;
            gluDeleteQuadric( m_GluObj );
        }

        /**
         * Returns the unique component ID
         */
        const string componentID() const 
        { 
            return string("gluquadric"); 
        }

        /**
         * Renders the model
         */
        virtual void render() const = 0;

    }; // end of OCGXModel class

} // end of namespace tlib