
#pragma once

#include <string>
#include "gluQuadric.h"
#include "presentation.h"

using std::string;

namespace tlib
{

    class OCGluSphere : public OCGluQuadric
    {
    private:
       float m_fRadius;
	   unsigned int m_uiSlices, m_uiStacks;

    public:
        /**
         * Constructors
         */
        OCGluSphere() {}
        OCGluSphere( GLfloat radius, GLuint slices, GLuint stacks )
        {
            create( radius, slices, stacks );
        }

        /**
         * Returns the unique component ID
         */
        const string componentID() const 
        { 
            return string("gluquadric"); 
        }

        /**
         * Creates the quadric object
         */
        void create( GLfloat radius, GLuint slices, GLuint stacks )
        {
            m_GluObj = gluNewQuadric();
	        gluQuadricNormals( m_GluObj, GLU_SMOOTH );

            m_fRadius   = radius;
	        m_uiSlices  = slices;
	        m_uiStacks  = stacks;
        }

        /**
         * Renders the model
         */
        virtual void render() const
        {
            if( !m_GluObj ) return;

            // get the object's position
            const vector3f_t& pos = getOwner()->getPos();

            // apply color/material/texture
            IComponent *comp = m_oOwner->getComponent("presentation");
            IOCPresentation *pres = static_cast<IOCPresentation*>(comp);
            if( pres )
                pres->apply();

            glPushMatrix();
                glTranslatef( pos.x(), pos.y(), pos.z() );
                gluSphere( m_GluObj, m_fRadius, m_uiSlices, m_uiStacks );
            glPopMatrix();
        }

    }; // end of OCGXModel class

} // end of namespace tlib