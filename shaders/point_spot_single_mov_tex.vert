/**
 * This file shades geometry with one spotlight and one to seven 
 * point lights.
 * The spotlight's colors are calculated here and attenuation in
 * the pixel shader whereas the point lights' colors are calculated
 * in the pixel shader and their attenuation here.
 */

// Point light outputs
const int MAX_LIGHTS = 3;
struct _PointLight {
    vec3 lightDir;
    float att;
};
varying _PointLight pointLights[MAX_LIGHTS];

// Spot light outputs
varying vec3 spot_lightDir, halfVector;
varying vec4 spot_diffuse, spot_ambient, ambientGlobal;
varying float spot_dist;

// General outputs
varying vec3 normal, eyeVec;

void main()
{	
    // Find vertex normal
	normal = gl_NormalMatrix * gl_Normal;

    // Transform vertex
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	// Set vertex to eye vector
	eyeVec = -vVertex;
	
	// Save global ambient light value
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	// _________________________________________________________________
	// Do calculations for the spotlight.
	// -----------------------------------------------------------------
	vec3 aux = vec3(gl_LightSource[0].position.xyz - vVertex);
	spot_lightDir = normalize(aux);
	spot_dist = length(aux);
	
	// Compute the diffuse, ambient and globalAmbient terms.
	spot_diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
	// The ambient terms have been separated since one of them
	// suffers attenuation.
	spot_ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	
	// Set the light source's half vector
	halfVector = normalize( gl_LightSource[0].halfVector.xyz );
	
	// _________________________________________________________________
	// Do calculations for the point lights
	// -----------------------------------------------------------------
	//_PointLight pointLights[MAX_LIGHTS];
	for( int i=0; i<MAX_LIGHTS; ++i )
	{
	    // Calculate light direction
	    pointLights[i].lightDir = vec3(gl_LightSource[i+1].position.xyz - vVertex);
	
	    // Calculate the distance between the light source and the 
	    // vertex. Used for attenuation.
	    float dist = length(pointLights[i].lightDir);
    	
	    // Calculate attenuation based on distance from light source
	    pointLights[i].att = 1.0 / 
	                ( gl_LightSource[i+1].constantAttenuation + 
	                ( gl_LightSource[i+1].linearAttenuation * dist ) + 
	                ( gl_LightSource[i+1].quadraticAttenuation * dist * dist ) );
	}
	
	// Send vertex
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();		
}