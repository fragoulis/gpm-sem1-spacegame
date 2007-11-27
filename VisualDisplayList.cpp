#include <windows.h>
#include <gl/gl.h>
#include "VisualDisplayList.h"
#include "Object.h"
#include "Material.h"
#include "Texture.h"
#include "Logger.h"

namespace tlib
{

    OCVisualDisplayList::~OCVisualDisplayList() {
        glDeleteLists( m_uiListId, 1 );
    }

    void OCVisualDisplayList::render() const
    {
        glPushMatrix();

        // get the object's position
        const Vector3f& pos = getOwner()->getPos();
        glTranslatef( pos.x(), pos.y(), pos.z() );

        // Apply material if component exists
        IOCMaterial *cMaterial = (IOCMaterial*)m_oOwner->getComponent("material");
        if( cMaterial )
            cMaterial->apply();

        // Apply texture if component exists
        IOCTexture *cTexture = (IOCTexture*)m_oOwner->getComponent("texture");
        if( cTexture )
            cTexture->apply();
            
        // Load transformation quaternion and apply rotations
        getOwner()->getDir().toMatrix(m_fRotMatrix);
        glMultMatrixf(m_fRotMatrix);

        glCallList( m_uiListId );

        // Turn of texturing in case texture component turned it on
        glDisable( GL_TEXTURE_2D );

        glPopMatrix();
    }

    void OCVisualDisplayList::generate()
    {
        _LOG("Generating display list...");
        m_uiListId = glGenLists(1);
        if( !glIsList(m_uiListId) ) {
            _LOG("Failed to generate display list(" 
                 +toStr<int>(m_uiListId)+ ")");
        } else {
            _LOG("Generated display list(" 
                 +toStr<int>(m_uiListId)+ ")");

            // Build display list
            glNewList( m_uiListId, GL_COMPILE );
                buildObject();
            glEndList();
        }

    } // end generate()

} // end of namespace tlib