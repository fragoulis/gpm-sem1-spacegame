#pragma once
#include "component.h"
#include "ShaderMgr.h"

namespace tlib
{

    class OCShader : public IComponent
    {
    private:
        // The shader program id
        unsigned m_uiProg;

        // Whether the shader program is currently in use
        bool m_bIsActive;

    public:
        /**
         * Constructor
         */
        OCShader( ShaderMgr::ShaderProgram iType );

        /**
         * Destructor
         */
        virtual ~OCShader();
        
        /**
         * Returns the component's family ID
         */
        const string familyID() const { 
            return string("shader"); 
        }

        /**
         * Returns the component's ID
         */
        const string componentID() const { 
            return string("shader"); 
        }

        /**
         * Returns the program id
         */
        unsigned getProgId() const { return m_uiProg; }

        /**
         * Starts the use of this program
         */
        void begin();

        /**
         * Stops the use of this program
         */
        void end();

        /**
         * Returns whether is active or not
         */
        bool isActive() const { return m_bIsActive; }

        /**
         * Returns the location of a variable inside the 
         * shader
         */
        unsigned getUniform( const char *var ) {
            return ShaderMgr::Instance().getUniform( m_uiProg, var );
        }

    }; // end of OCShader class

} // end of namespace tlib