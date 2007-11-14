#pragma once
#include "Component.h"
#include <map>
#include <string>
using std::map;
using std::string;

namespace tlib
{
    template<class T>
    class OCComponentContainer : public IComponent
    {
        typedef map< string, T* > ComponentMap;

    private:
        ComponentMap m_Container;

    public:
        OCComponentContainer(){}
        ~OCComponentContainer()
        {
            clearComponentList();
        }

        /**
         * Frees all memory that the components occupy
         */
        void clearComponentList()
        {
            ComponentMap::iterator iter;
            for( iter = m_Container.begin(); 
                 iter != m_Container.end(); 
                 iter++ )
            {
                delete iter->second;
                iter->second = 0;
            }
        }

        void pushComponent( const string& key, T *comp )
        {
            // set the owner of this component to be this
            comp->setOwner( egtOwner() );
            m_Container[ key ] = comp;
        }

        T* getComponent( const string& key )
        {
            return m_Container[ key ];
        }
    };
}