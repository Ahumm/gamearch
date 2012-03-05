#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec4 in_Normal;

uniform float time;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 v_tex;

void main( void )
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position.xyz, 1.0);
	v_tex = in_Tex;
}
