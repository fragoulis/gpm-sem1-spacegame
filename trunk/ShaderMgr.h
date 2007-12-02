#pragma once
#include <gl/gl.h>
#include "Singleton.h"
#include <vector>
using std::vector;

namespace tlib
{
    
    class ShaderMgr : public Singleton<ShaderMgr>
    {
        friend Singleton<ShaderMgr>;

    private:
        // This struct holds a program handle and two shader handles
        // This will help us keep track of our shaders
        struct _Shader {
            _Shader( GLuint _uiProg, 
                     GLuint _uiVertex, 
                     GLuint _uiPixel ):
            uiProg(_uiProg),
            uiVertex(_uiVertex),
            uiPixel(_uiPixel)
            {}
                
            GLuint uiProg;
            GLuint uiVertex;
            GLuint uiPixel;
        };

        typedef vector<_Shader> ShaderList;

        // A list of shaders and programs
        ShaderList m_vList;

        // Return codes
        static const int INVALID_FILENAME;
        static const int COMPILE_FAILED;

    public:
        /**
         * Adds a new shader program
         */
        GLuint addProgram( const char *vertex, const char *pixel );

        /**
         * Get index of a uniform variable
         */
        int getUniform( GLuint uiProg, const char *name );

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
        GLuint addShader( GLuint shaderType, const char *file );

        /**
         * Loads the source of a shader from file
         */
        char* loadSource( const char *file );

    }; // end of ShaderMgr class

} // end of namspace tlib