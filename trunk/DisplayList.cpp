
#include <windows.h>
#include <gl/gl.h>
#include "DisplayList.h"
#include "Object.h"
#include "Presentation.h"
#include "Logger.h"

namespace tlib
{

    OCDisplayList::~OCDisplayList() {
        _LOG("Deleting display list...");
        glDeleteLists( m_uiListId, 1 );
    }

    void OCDisplayList::render() const
    {
        // get the object's position
        const Vector3f& pos = getOwner()->getPos();

        // apply color/material/texture
        IComponent *comp = m_oOwner->getComponent("presentation");
        IOCPresentation *pres = static_cast<IOCPresentation*>(comp);
        if( pres )
            pres->apply();

        glPushMatrix();
            glTranslatef( pos.x(), pos.y(), pos.z() );
            glCallList( m_uiListId );
        glPopMatrix();
    }

    void OCDisplayList::generate()
    {
        _LOG("Generating display list...");
        m_uiListId = glGenLists(1);
        glNewList( m_uiListId, GL_COMPILE );
            buildObject();
        glEndList();
    }

} // end of namespace tlib