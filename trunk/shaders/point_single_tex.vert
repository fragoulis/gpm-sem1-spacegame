varying vec3 normal, lightDir, eyeVec;
varying float att;

void main()
{	
    int i = 0;
    
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[i].position.xyz - vVertex);
	eyeVec = -vVertex;
	
	float d = length(lightDir);
	
	att = 1.0 / ( gl_LightSource[i].constantAttenuation + 
	(gl_LightSource[i].linearAttenuation*d) + 
	(gl_LightSource[i].quadraticAttenuation*d*d) );
	
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();		
}