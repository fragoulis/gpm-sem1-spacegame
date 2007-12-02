#include <GL/GLee.h>
#include "ShaderMgr.h"
#include "Logger.h"
#include <cstdio>

namespace tlib
{

    const int ShaderMgr::INVALID_FILENAME  = -1;
    const int ShaderMgr::COMPILE_FAILED    = -2;

    ShaderMgr::ShaderMgr()
    {}

    ShaderMgr::~ShaderMgr()
    {
        // Clean up shaders
        ShaderList::const_iterator iter;
        for( iter = m_vList.begin(); iter != m_vList.end(); ++iter )
        {
            const _Shader& sh = *iter;
            glDetachShader( sh.uiProg, sh.uiVertex );
            glDetachShader( sh.uiProg, sh.uiPixel );
            glDeleteShader( sh.uiVertex );
            glDeleteShader( sh.uiPixel );
            glDeleteProgram( sh.uiProg );
        }
    }

    // ------------------------------------------------------------------------
    GLuint ShaderMgr::addProgram( const char *vertex, const char *pixel )
    {

        // Load vertex shader
        GLuint uiVertex = addShader( GL_VERTEX_SHADER, vertex );
        switch( uiVertex ) {
            case INVALID_FILENAME: 
                printf("Failed to load vertex shader from file\n");
                return 0;
                break;
            case COMPILE_FAILED: 
                printf("Failed to compile vertex shader\n");
                return 0;
                break;
        }

        // Load pixel shader
        GLuint uiPixel = addShader( GL_FRAGMENT_SHADER, pixel );
        switch( uiPixel ) {
            case INVALID_FILENAME: 
                printf("Failed to load pixel shader from file\n");
                return 0;
                break;
            case COMPILE_FAILED: 
                printf("Failed to compile pixel shader\n");
                return 0;
                break;
        }

        // Create program object
        GLuint uiProg = glCreateProgram();

        // Attach shaders to program
        glAttachShader( uiProg, uiVertex );
        glAttachShader( uiProg, uiPixel );

        // Link program
        glLinkProgram( uiProg );

        // Check if it linked
        int iDone = 0;
	    glGetProgramiv( uiProg, GL_LINK_STATUS, &iDone );
	    if( !iDone ) {
            // Clean up
            glDetachShader( uiProg, uiVertex );
            glDetachShader( uiProg, uiPixel );
            glDeleteShader( uiVertex );
            glDeleteShader( uiPixel );
            glDeleteProgram( uiProg );
		    printf("Failed to link shader program\n");
		    return 0;
	    }
        
        // Push the program to the list
        m_vList.push_back( _Shader( uiProg, uiVertex, uiPixel ) );

        return uiProg;
    }

    // ------------------------------------------------------------------------
    GLuint ShaderMgr::addShader( GLenum shaderType, const char *file )
    {
        // Load shader's source from
        const char *source = loadSource( file );
        if( !source ) {
            delete [] source;
            return INVALID_FILENAME;
        }

        // Create the shader object
        GLuint uiHandle = glCreateShader( shaderType );

        // Load shader's source to object
        glShaderSource( uiHandle, 1, &source, NULL );

        // Compile shader
        glCompileShader( uiHandle );

        // Check if it compiled correctly
        int iDone = 0;
        glGetShaderiv( uiHandle, GL_COMPILE_STATUS, &iDone );
	    if( !iDone ) {
            glDeleteShader( uiHandle );
		    delete [] source;
            return COMPILE_FAILED;
	    }

        // Clean up
        delete [] source;

        // Return the shader's handle
        return uiHandle;

    } // end addShader()

    // ------------------------------------------------------------------------
    char* ShaderMgr::loadSource( const char *filename ) 
    {
	    if( !filename ) return NULL;

        FILE *fp;
	    if( fopen_s( &fp, filename, "r" ) )
            return NULL;

	    // seek to end of file
	    fseek(fp, 0, SEEK_END);
	    // how long is the file?
	    long size = ftell(fp);
	    // rewind
	    fseek(fp, 0, SEEK_SET);
    	
	    // allocate
	    char *buff = new char[size+1];
	    if( !buff ) {
		    printf("LoadSource: failed to allocate %ld bytes\n", size);
		    fclose(fp);
		    return NULL;
	    }
	    // read shader from file
	    size_t count = fread(buff, 1, size, fp);
	    // null terminate
	    buff[count] = 0;
	    // check for errors
	    if( !count || ferror(fp) ) {
		    delete [] buff;
		    buff=NULL;
	    }

	    // close file
	    fclose(fp);

	    // return
	    return buff;

    } // end loadSource()

    // ------------------------------------------------------------------------
    int ShaderMgr::getUniform( GLuint uiProg, const char *name ) 
    {
        if( !uiProg || !name ) return 0;

        int i = glGetUniformLocation( uiProg, name );
        if( i < 0 )
	        printf("GetUniform: uniform not found (%s)\n", name);

        return i;
    }

} // end of namspace tlib