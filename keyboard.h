
#pragma once
#include "controller.h"
#include "assert.h"

namespace tlib
{

    class OCKeyboard : public IOCController
    {
    private:
        int     m_iKey;
        bool    m_bDown;
        bool    m_vbKeys[256];

    public:
        /**
         * Constructor
         */
        OCKeyboard(): m_iKey(0), m_bDown(false)
        {
            memset( m_vbKeys, 0, sizeof(m_vbKeys) );
        }

        /**
         * Destructor
         */
        virtual ~OCKeyboard() {}

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("keyboard"); 
        }

        /**
         * Reads and saves the keyboard input
         */
        void capture( int key, bool down )
        {
            key = tolower(key);
            
            // Save the keys
            m_iKey        = key;
            m_bDown       = down;
            m_vbKeys[key] = down;
        }

        int getKey() const { return m_iKey; }
        bool isDown() const { return m_bDown; }
        int getKey( int k ) const 
        { 
            _ASSERT(k>=0&&k<256);
            return m_vbKeys[k]; 
        }

        /**
         * Executes after capturing user's input user input
         */
        virtual void update() const = 0;

    }; // end of OCKeyboard class

} // end of namespace tlib
