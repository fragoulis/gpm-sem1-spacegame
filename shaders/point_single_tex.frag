uniform sampler2D colorMap;
varying vec3 normal, lightDir, eyeVec;
varying float att;

void main (void)
{
    int i = 0;
    
	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[i].ambient * gl_FrontMaterial.ambient)*att;
							
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
    	
	if(lambertTerm > 0.0)
	{
	    vec4 texture = texture2D( colorMap, gl_TexCoord[0].st );
		final_color += gl_LightSource[i].diffuse * 
		               gl_FrontMaterial.diffuse * 
		               texture *
		               lambertTerm * 
		               att;	
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		
		float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		
		final_color += gl_LightSource[i].specular * 
		               gl_FrontMaterial.specular * 
		               specular * 
		               att;	
	}

	gl_FragColor = final_color;
}