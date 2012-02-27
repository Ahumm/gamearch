#version 150

precision highp float;

in vec2 v_tex;
uniform sampler2D s_tex;

out vec4 FragColor;

void main(void)
{
	FragColor = texture(s_tex, v_tex);
    //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}