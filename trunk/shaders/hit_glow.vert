uniform float glow_timer, noise_timer;

void main()
{   
    gl_FrontColor = vec4( 1.0 - glow_timer, 
                          0.0, 
                          noise_timer, 
                          0.6 - glow_timer * 0.2 );
    
    /*vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 newPos = vec3( vertex.x + sin(noise_timer), 
                        vertex.y, 
                        vertex.z );*/
                        
    /*vec4 newPos = vec4( gl_Vertex.x * 1.1, 
                        gl_Vertex.y / 2.0, 
                        gl_Vertex.z * 1.3, 
                        gl_Vertex.w );*/
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    //gl_Position = gl_ModelViewProjectionMatrix * newPos;
    gl_Position = ftransform();
}