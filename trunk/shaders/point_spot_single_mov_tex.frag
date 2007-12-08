/**
 * This file shades geometry with one spotlight and one to seven 
 * point lights.
 * We have different inputs for the spot light and the point lights.
 */

// Point light inputs
const int MAX_LIGHTS = 3;
struct _PointLight {
    vec3 lightDir;
    float att;
};
varying _PointLight pointLights[MAX_LIGHTS];

// The surfaces texture
uniform sampler2D colorMap;
// Timer for the texture animation
uniform float timer, glow_timer;

// Spot light inputs
varying vec3 spot_lightDir, halfVector;
varying vec4 spot_diffuse, spot_ambient, ambientGlobal;
varying float spot_dist;

// General inputs
varying vec3 normal, eyeVec;
 
void main (void)
{
    // Get texture color
    vec2 displace = vec2( 0, cos(timer)*sin(timer)*0.6 );
    vec4 texture = texture2D( colorMap, gl_TexCoord[0].st + displace.st ) + 
                   vec4( displace, displace );
	
	/* Combine two ambient terms, one of the spot light and one for all
	 * other point lights.
	 * FOr simplicity, we say that light's ambient is always black
	 */
	vec4 final_color = 
	    ambientGlobal + 
	    gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient;
	    
	
	vec3 N = normalize(normal);
	
	// ______________________________________________________________
	// Calculate lamberTerm for the spotlight
	float spot_lambertTerm = max( dot( N, normalize(spot_lightDir) ), 0.0 );
	
	if( spot_lambertTerm > 0.0 ) 
	{    
	    // Spot effect: spotDir . -lightDir
	    float spotEffect = 
	        dot( normalize(gl_LightSource[0].spotDirection), normalize(-spot_lightDir) );
	        
		if( spotEffect > gl_LightSource[0].spotCosCutoff ) 
		{
			// Compute the illumination in here
		    spotEffect = pow( spotEffect, gl_LightSource[0].spotExponent );
		    
		    float spot_att = 
		        spotEffect / ( gl_LightSource[0].constantAttenuation +
				             ( gl_LightSource[0].linearAttenuation * spot_dist  ) +
				             ( gl_LightSource[0].quadraticAttenuation * spot_dist * spot_dist ) );
    			
		    final_color += spot_att * 
		                   ( spot_diffuse * texture * spot_lambertTerm + spot_ambient );
    		
		    vec3 halfV = normalize(halfVector);
		    float NdotHV = max( dot( N, halfV ), 0.0 );
		    final_color += spot_att * 
		                   gl_FrontMaterial.specular * 
				           gl_LightSource[0].specular * 
				           pow( NdotHV, gl_FrontMaterial.shininess );
		}
	}
	
	// ______________________________________________________________
	// Calculate lamberTerm for the point lights
	vec3 E = normalize(eyeVec);
	vec4 cacheDiffuse = gl_FrontMaterial.diffuse * texture;
	
	for( int i=0; i<MAX_LIGHTS; ++i )
	{
	    final_color += gl_LightSource[i].ambient * 
	                   gl_FrontMaterial.ambient * 
	                   pointLights[i].att;
	    
	    vec3 L = normalize(pointLights[i].lightDir);
        float lambertTerm = dot(N,L);

	    if( lambertTerm > 0.0 )
	    {
		    final_color += gl_LightSource[i+1].diffuse * 
		                   lambertTerm *
		                   cacheDiffuse * 
		                   pointLights[i].att;
    		
		    vec3 R = reflect(-L, N);
    		
		    float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
    		
		    final_color += gl_LightSource[i+1].specular * 
		                   gl_FrontMaterial.specular * 
		                   specular *
		                   pointLights[i].att;	
	    }
	}
                   
    const vec4 GLOW = vec4(glow_timer,glow_timer,glow_timer,1.0);
	gl_FragColor = final_color + GLOW;
}