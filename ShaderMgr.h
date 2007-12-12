#pragma once
#include "Singleton.h"
#include <vector>
using std::vector;

namespace tlib
{
    class ShaderMgr : public Singleton<ShaderMgr>
    {
        friend Singleton<ShaderMgr>;

    public:
        // An enumeration of the shader programs we will use
        enum ShaderProgram {
            POINT_LIGHT_SINGLE_TEX = 0,
            SPOT_LIGHT_SINGLE_TEX,
            POINT_AND_SPOT_LIGHT_SINGLE_TEX,
            POINT_AND_SPOT_LIGHT_NO_TEX,
            FORCEFIELD_DOUBLE_TEX,
            BUMP_POINT_LIGHT,
            POINT_AND_SPOT_SINGLE_MOV_TEX,
            HIT_GLOW,
            GLOW_MOV_TEXT,
            NUM_OF_SHADERS
        };

    private:
        // This struct holds a program handle and two shader handles
        // This will help us keep track of our shaders
        struct _Shader {
            _Shader( unsigned _uiProg, 
                     unsigned _uiVertex, 
                     unsigned _uiPixel ):
            uiProg(_uiProg),
            uiVertex(_uiVertex),
            uiPixel(_uiPixel)
            {}
                
            unsigned uiProg;
            unsigned uiVertex;
            unsigned uiPixel;
        };

        typedef vector<_Shader> ShaderList;

        // The list of programmes
        unsigned int m_vProgIds[NUM_OF_SHADERS];

        // The program id in use
        unsigned int m_uiActiveProg;

        // A list of shaders and programs
        ShaderList m_vList;

        // Return codes
        static const int INVALID_FILENAME;
        static const int COMPILE_FAILED;

    public:
        /**
         * Initializes the manager by creating all programs
         */
        void init();

        /**
         * Returns the requested program id
         */
        unsigned int getProgram( ShaderProgram iType ) const {
            _ASSERT((iType>=0)&&(iType<NUM_OF_SHADERS));
            return m_vProgIds[ iType ];
        }

        /**
         * Get index of a uniform variable
         */
        int getUniform( unsigned uiProg, const char *name );
        int getUniform( const char *name );

        /**
         * Starts the use of a program
         */
        void begin( ShaderProgram iType );

        /**
         * Stops the use of a program
         */
        void end();

        void printShaderInfoLog(ShaderProgram iType);
	    void printProgramInfoLog(ShaderProgram iType);

    private:
        /**
         * Constructor
         */
        ShaderMgr();

        /**
         * Destructor
         */
        ~ShaderMgr();

        /**
         * Creates a vertex shader, compiles it and attaches it
         * to a program
         */
        int addShader( unsigned shaderType, const char *file );

        /**
         * Loads the source of a shader from file
         */
        char* loadSource( const char *file );

        /**
         * Adds a new shader program by type
         */
        bool addProgram( ShaderProgram iType, 
                         const char *vertex, const char *pixel );

    }; // end of ShaderMgr class

} // end of namspace tlib