
void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_FrontMaterial.diffuse;
    gl_Position = ftransform();
}