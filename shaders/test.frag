
uniform sampler2D colorMap;

void main()
{
    gl_FragColor = texture2D( colorMap, gl_TexCoord[0].st) + gl_Color;
}