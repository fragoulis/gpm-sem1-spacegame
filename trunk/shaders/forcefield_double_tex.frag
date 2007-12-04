
uniform sampler2D noiseMap, colorMap;
uniform float timer;

void main()
{
    vec3 noiseVec;
	vec2 displacement;
	float scaledTimer;

	displacement = gl_TexCoord[0].st;

	scaledTimer = timer*0.1;

	displacement.x += scaledTimer;
	displacement.y -= scaledTimer;

	noiseVec = normalize(texture2D(noiseMap, displacement.xy).xyz);
	noiseVec = (noiseVec * 2 - 1.0) * 0.035;
	
	gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st + noiseVec.xy) * gl_Color;
}