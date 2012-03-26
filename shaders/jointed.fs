#version 150

in vec4 v_Color;
in vec2 v_Tex;
in bool tex_or_color; // True for texture, false for color

uniform Sampler2D s_tex;

out vec4 FragColor;

void main( void )
{
    if(tex_or_color)
    {
        FragColor = v_Color;
    }
    else
    {
        FragColor = texture(s_tex, v_tex);
    }
}