//  Set the fragment color

uniform sampler2D tex;

void main()
{
    vec4 color = gl_Color * texture2D(tex,gl_TexCoord[0].xy);
    if(color.g < 0.1 && color.r < 0.1 && color.b <0.1)
        discard;


   gl_FragColor = color;
}
