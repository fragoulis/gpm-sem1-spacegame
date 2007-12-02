#include <gl/glee.h>
#include "VisualBox.h"
#include "Texture.h"
#include "Object.h"

namespace tlib
{
    OCVisualBox::OCVisualBox()
    {}

    OCVisualBox::OCVisualBox( const Vector3f& vBBox ): 
    m_vHalfDim(vBBox)
    {
        generate();
    }

    // ------------------------------------------------------------------------
    void OCVisualBox::init( const Vector3f& vBBox )
    {
        m_vHalfDim = vBBox;
        generate();
    }

    void OCVisualBox::texCoord( int iTextures, float x, float y ) const 
    {
        if( !iTextures ) return;
        if( iTextures == 1 )
            glTexCoord2f( x, y );
        else {
            for( int i=0; i<iTextures; ++i ) {
                glMultiTexCoord2f( GL_TEXTURE0+i, x, y );
            }
        }
    }

    // ------------------------------------------------------------------------
    void OCVisualBox::buildObject() const
    {
        // See if the owner has any textures
        int iTextures = 0;
        IOCTexture *cTex = 0;
        if( m_oOwner )
        {
            cTex = (IOCTexture*)m_oOwner->getComponent("texture");
            if( cTex ) {
                iTextures = (int)cTex->getNumOfTextures();
            }
        }

        glBegin(GL_QUADS);
        {
            // front face
            glNormal3f( 0.0f, 0.0f, 1.0f );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f(  m_vHalfDim.x(),  m_vHalfDim.y(), m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), m_vHalfDim.z() );

            // back face
            glNormal3f( 0.0f, 0.0f, -1.0f );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f(  m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );

            // top face
            glNormal3f( 0.0f, 1.0f, 0.0f );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(), m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f(  m_vHalfDim.x(), m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f(  m_vHalfDim.x(), m_vHalfDim.y(), -m_vHalfDim.z() );

            // bottom face
            glNormal3f( 0.0f, -1.0f, 0.0f );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f(  m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );

            // right face
            glNormal3f( 1.0f, 0.0f, 0.0f );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f( m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f( m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f( m_vHalfDim.x(),  m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f( m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );

            // left face
            glNormal3f( -1.0f, 0.0f, 0.0f );
            texCoord( iTextures, 0.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(), -m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 0.0f );
            glVertex3f( -m_vHalfDim.x(), -m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 1.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(),  m_vHalfDim.z() );
            texCoord( iTextures, 0.0f, 1.0f );
            glVertex3f( -m_vHalfDim.x(),  m_vHalfDim.y(), -m_vHalfDim.z() );
        }
        glEnd();

    } // end buildObject()


} // end of namespace tlib