#include <windows.h>
#include <gl/gl.h>
#include "VisualBox.h"

namespace tlib
{
    OCVisualBox::OCVisualBox( const Vector3f& vHalfDim ): 
      m_vHalfDim(vHalfDim)
    {
        generate();
    }

    void OCVisualBox::buildObject() const
    {
        glBegin(GL_QUADS);
        {
            // back face
            glNormal3f( 0.0f, 0.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(),  m_vHalfDim.y(), m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), m_vHalfDim.z() );

            // front face
            glNormal3f( 0.0f, 0.0f, -1.0f );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );

            // right face
            glNormal3f( 1.0f, 0.0f, 0.0f );
            glVertex3f( m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f( m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f( m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f( m_vHalfDim.x(),  m_vHalfDim.y(),  m_vHalfDim.z() );

            // left face
            glNormal3f( -1.0f, 0.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );

            // top face
            glNormal3f( 0.0f, 1.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(), m_vHalfDim.y(), -m_vHalfDim.z() );

            // bottom face
            glNormal3f( 0.0f, -1.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
        }
        glEnd();

    } // end buildObject()

} // end of namespace tlib