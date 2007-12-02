#include "VisualGXModel.h"
#include "Object.h"
#include "Vector3.h"
#include "QuatRotation.h"
#include "Logger.h"

namespace tlib
{
    OCGXModel::OCGXModel() {}
    OCGXModel::OCGXModel( const char *filename ) {
        load( filename, 1.0f );
    }
    OCGXModel::OCGXModel( const char *filename, float scale ) {
        load( filename, scale );
    }

    OCGXModel::~OCGXModel() {
        //m_Model.Free();
        m_Model.FreeGL();
    }

    // ------------------------------------------------------------------------
    bool OCGXModel::load( const char *filename, float scale )
    {
        if( !m_Model.Load( filename ) ) {
            _LOG("Loading model " + string(filename) + " failed");
            return false;
        }

        if( scale != 1.0f )
            m_Model.SetScale( scale );

        return true;
    }

    // ------------------------------------------------------------------------
    void OCGXModel::render() const
    {
        glPushAttrib(GL_ENABLE_BIT);
        glPushMatrix();
        {
            // Get object's position
            const Vector3f& vPos = getOwner()->getPos();

            // Place the object
            glTranslatef( vPos.x(), vPos.y(), vPos.z() );
#ifdef _SHOW_AXIS
            IComponent *cmp = getOwner()->getComponent("orientation");
            OCOrientation3D *qr = (OCOrientation3D*)cmp;
            if( qr ) {
                const Vector3f& view = qr->getView() * 20.0f;
                const Vector3f& right = qr->getRight() * 20.0f;
                const Vector3f& up = qr->getUp() * 20.0f;
                
                glDisable(GL_LIGHTING);
                glBegin(GL_LINES);
                    glColor3f(1,0,0);
                    glVertex3f(0,0,0);
                    glVertex3f(view.x(), view.y(), view.z());
                    glColor3f(0,1,0);
                    glVertex3f(0,0,0);
                    glVertex3f(right.x(), right.y(), right.z());
                    glColor3f(0,0,1);
                    glVertex3f(0,0,0);
                    glVertex3f(up.x(), up.y(), up.z());
                glEnd();
                glEnable(GL_LIGHTING);
            }
#endif
            // Load transformation quaternion and apply rotations
            getOwner()->getDir().toMatrix(m_fRotMatrix);
            glMultMatrixf(m_fRotMatrix);

            // Render the model
            m_Model.DrawGL();
        }
        glPopMatrix();
        glPopAttrib();
    }

} // end of namespace tlib


/*IComponent *cmp = getOwner()->getComponent("orientation");
            OCQuatRotation *qr = (OCQuatRotation*)cmp;
            const Vector3f& view = qr->getView() * 20.0f;
            const Vector3f& right = qr->getRight() * 20.0f;
            const Vector3f& up = qr->getUp() * 20.0f;
            
            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
                glColor3f(1,0,0);
                glVertex3f(0,0,0);
                glVertex3f(view.x(), view.y(), view.z());
                glColor3f(0,1,0);
                glVertex3f(0,0,0);
                glVertex3f(right.x(), right.y(), right.z());
                glColor3f(0,0,1);
                glVertex3f(0,0,0);
                glVertex3f(up.x(), up.y(), up.z());
            glEnd();
            glEnable(GL_LIGHTING);*/

            //float 
            //    cx = m_Model.GetBoxCentre().x,
            //    cy = m_Model.GetBoxCentre().y,
            //    cz = m_Model.GetBoxCentre().z,
            //    sx = m_Model.GetBoxSize().x/2,
            //    sy = m_Model.GetBoxSize().y/2,
            //    sz = m_Model.GetBoxSize().z/2;
            //glPushMatrix();
            //    glTranslatef(cx,cy,cz);
            //    glDisable(GL_LIGHTING);
            //    glDisable(GL_CULL_FACE);
            //    glBegin(GL_QUADS);
            //        glColor3f(1,1,0);
            //        glNormal3f( 0,1,0 );
            //        glVertex3f(-sx,sy, sz);
            //        glVertex3f( sx,sy, sz);
            //        glVertex3f( sx,sy,-sz);
            //        glVertex3f(-sx,sy,-sz);
            //        glNormal3f( 0,-1,0 );
            //        glVertex3f(-sx,-sy,-sz);
            //        glVertex3f( sx,-sy,-sz);
            //        glVertex3f( sx,-sy, sz);
            //        glVertex3f(-sx,-sy, sz);
            //    glEnd();
            //    glEnable(GL_LIGHTING);
            //    glEnable(GL_CULL_FACE);
            //glPopMatrix();