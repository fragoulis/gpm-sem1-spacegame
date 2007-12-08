void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Color = gl_FrontColor;
    gl_Position = ftransform();
}