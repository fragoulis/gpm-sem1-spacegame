#pragma once
#include "Component.h"

namespace tlib
{

    class IOCVitals : public IComponent
    {
    protected:
        // Boolean value that indicates whether we should
        // keep checking this object's vitals
        bool m_bHasEnded; 

    public:
        /**
         * Construtor
         */
        IOCVitals();

        /**
         * Destructor
         */
        virtual ~IOCVitals();
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("vitals"); 
        }

        /**
         * Accessors
         */
        bool hasEnded() const { return m_bHasEnded; }

        /**
         * Retunrs whether this object is alive
         */
        virtual bool isAlive() const = 0;

        /**
         * Updates and checks the vitals of an object
         */
        virtual void update() = 0;

        /**
         * This is called on an object's death
         */
        virtual void onKill() = 0;

        /**
         * This is called on an object's revival
         */
        virtual void onRevive() = 0;

    };

} // end of namespace tlib