#include "gl/glee.h"
#include "ShaderMgr.h"
#include "Logger.h"
#include <cstdio>

namespace tlib
{

    const int ShaderMgr::INVALID_FILENAME  = -1;
    const int ShaderMgr::COMPILE_FAILED    = -2;

    ShaderMgr::ShaderMgr():
    m_uiActiveProg(0)
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
    void ShaderMgr::begin( ShaderProgram iType )
    {
        _ASSERT((iType>=0)&&(iType<NUM_OF_SHADERS));
        m_uiActiveProg = m_vProgIds[ iType ];
        glUseProgram( m_uiActiveProg );
        
    }

    // ------------------------------------------------------------------------
    void ShaderMgr::end() {
        glUseProgram(0);
        m_uiActiveProg = 0;
    }

    // ------------------------------------------------------------------------
    void ShaderMgr::init()
    {
        if( !addProgram( POINT_LIGHT_SINGLE_TEX, 
                         "shaders/point_single_tex.vert", 
                         "shaders/point_single_tex.frag" ) ) {
            _LOG("POINT_LIGHT_SINGLE_TEX shader program failed!");
        }
        if( !addProgram( SPOT_LIGHT_SINGLE_TEX, 
                         "shaders/spot_single_tex.vert", 
                         "shaders/spot_single_tex.frag" ) ) {
            _LOG("SPOT_LIGHT_SINGLE_TEX shader program failed!");
        }
        if( !addProgram( POINT_AND_SPOT_LIGHT_SINGLE_TEX, 
                         "shaders/point_spot_single_tex.vert", 
                         "shaders/point_spot_single_tex.frag" ) ) {
            _LOG("POINT_AND_SPOT_LIGHT_SINGLE_TEX shader program!");
        }
        if( !addProgram( FORCEFIELD_DOUBLE_TEX, 
                         "shaders/forcefield_double_tex.vert", 
                         "shaders/forcefield_double_tex.frag" ) ) {
            _LOG("FORCEFIELD_DOUBLE_TEX shader program failed!");
        }
        if( !addProgram( POINT_AND_SPOT_LIGHT_NO_TEX, 
                         "shaders/point_spot_no_tex.vert", 
                         "shaders/point_spot_no_tex.frag" ) ) {
            _LOG("POINT_AND_SPOT_LIGHT_NO_TEX shader program!");
        }
        if( !addProgram( BUMP_POINT_LIGHT, 
                         "shaders/bump_point_light.vert", 
                         "shaders/bump_point_light.frag" ) ) {
            _LOG("BUMP_POINT_LIGHT shader program!");
        }
        if( !addProgram( POINT_AND_SPOT_SINGLE_MOV_TEX, 
                         "shaders/point_spot_single_mov_tex.vert", 
                         "shaders/point_spot_single_mov_tex.frag" ) ) {
            _LOG("POINT_AND_SPOT_SINGLE_MOV_TEX shader program!");
        }
        if( !addProgram( HIT_GLOW, 
                         "shaders/hit_glow.vert", 
                         "shaders/hit_glow.frag" ) ) {
            _LOG("HIT_GLOW shader program!");
        }
        if( !addProgram( GLOW_MOV_TEXT, 
                         "shaders/glow_mov_tex.vert", 
                         "shaders/glow_mov_tex.frag" ) ) {
            _LOG("GLOW_MOV_TEXT shader program!");
        }
    }

    // ------------------------------------------------------------------------
    bool ShaderMgr::addProgram( ShaderProgram iType, 
                                const char *vertex, const char *pixel )
    {
        _ASSERT((iType>=0)&&(iType<NUM_OF_SHADERS));

        // Load vertex shader
        GLuint uiVertex = addShader( GL_VERTEX_SHADER, vertex );
        switch( uiVertex ) {
            case INVALID_FILENAME: 
                _LOG("Failed to load vertex shader from file");
                return false;
                break;
            case COMPILE_FAILED: 
                _LOG("Failed to compile vertex shader");
                return false;
                break;
        }

        // Load pixel shader
        GLuint uiPixel = addShader( GL_FRAGMENT_SHADER, pixel );
        switch( uiPixel ) {
            case INVALID_FILENAME: 
                _LOG("Failed to load pixel shader from file");
                return false;
                break;
            case COMPILE_FAILED: 
                _LOG("Failed to compile pixel shader");
                return false;
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
		    _LOG("Failed to link shader program");
		    return false;
	    }
        
        // Push the program to the list
        m_vList.push_back( _Shader( uiProg, uiVertex, uiPixel ) );

        // Save program id to the program list
        m_vProgIds[ iType ] = uiProg;

        return true;
    }

    // ------------------------------------------------------------------------
    int ShaderMgr::addShader( GLenum shaderType, const char *file )
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

    // ------------------------------------------------------------------------
    int ShaderMgr::getUniform( const char *name ) 
    {
        if( !m_uiActiveProg || !name ) return 0;

        int i = glGetUniformLocation( m_uiActiveProg, name );
        if( i < 0 )
	        printf("GetUniform: uniform not found (%s)\n", name);

        return i;
    }

    // ------------------------------------------------------------------------
    void ShaderMgr::printShaderInfoLog(ShaderProgram iType)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetShaderiv(m_vProgIds[ iType ], GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetShaderInfoLog(m_vProgIds[ iType ], infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
	}

    // ------------------------------------------------------------------------
	void ShaderMgr::printProgramInfoLog(ShaderProgram iType)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetProgramiv(m_vProgIds[ iType ], GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetProgramInfoLog(m_vProgIds[ iType ], infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
	}

} // end of namspace tlib