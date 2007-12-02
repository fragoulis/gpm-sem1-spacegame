
#pragma once

#include <string>
using std::string;

namespace tlib
{
    // Forward declaration of Object class
    class Object;

    /**
     * This class is the most basic interface from which all object
     * component derive.
     */
    class IComponent
    {
    protected:
        // The component's owner object
        Object *m_oOwner;

    public:
        /**
         * Constructor
         */
        IComponent(): m_oOwner(0) {}

        /**
         * Destructor
         */
        virtual ~IComponent() = 0 {}

        /**
         * Virtual method that will return a unique component ID
         */
        virtual const string componentID() const = 0;
        
        /**
         * Virtual method that will return the component's family ID
         */
        virtual const string familyID() const = 0;

        /**
         * Returns the component's owner object
         */
        const Object* getOwner() const { return m_oOwner; }
        Object* getOwner() { return m_oOwner; }
        
        /**
         * Assigns an owner object to the component
         */
        void setOwner( Object *owner ) { m_oOwner = owner; }

    }; // end of class IComponent

} // end of namespace tlib