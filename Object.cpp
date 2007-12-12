#include "Object.h"

namespace tlib
{
    Object::Object(): 
    m_bActive(true),
    m_iType(DEFAULT)
    {}

    Object::~Object() {
        clearComponentList();           
    }

    /**
     * Frees all memory that the components occupy
     */
    void Object::clearComponentList()
    {
        component_array::iterator iter;
        for( iter = m_vComponents.begin(); 
             iter != m_vComponents.end(); 
             ++iter )
        {
			IComponent *comp = iter->second;
            delete comp;
            comp = 0;
        }
    }

    /**
     * Pushes a component to the component list
     */
    IComponent* Object::setComponent( IComponent *comp ) 
    { 
        // Get the component that MAY already exists of the
        // same interface
        IComponent *oldComp = m_vComponents[ comp->familyID() ];

        // set the owner of this component to be this
        comp->setOwner( this );

        // push the component to the list
        m_vComponents[ comp->familyID() ] = comp;

        return oldComp;
    }

} // end of namespace tlib