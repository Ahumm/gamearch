#version 150

in vec4 in_Position;
in vec4 in_Color;
in vec2 in_Tex;
in vec4 in_Normal;
in ivec3 in_Joints;
in vec3 in_Weights;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Joints[32];

out vec4 v_Color;
out vec2 v_Tex;

void main( void )
{
    mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
    
    vec4 t_joint1 = vec4(0.0);
    vec4 t_joint2 = vec4(0.0);
    vec4 t_joint3 = vec4(0.0);
    
    if(in_Joints[0] > -1) t_joint1 = (Joints[in_Joints[0]] * vec4(in_Position.xyz, 1.0)) * in_Weights[0];
    if(in_Joints[1] > -1) t_joint2 = (Joints[in_Joints[1]] * vec4(in_Position.xyz, 1.0)) * in_Weights[1];
    if(in_Joints[2] > -1) t_joint3 = (Joints[in_Joints[2]] * vec4(in_Position.xyz, 1.0)) * in_Weights[2];

    vec4 t_joint = t_joint1 + t_joint2 + t_joint3;
    
    t_joint.w = 1.0; // Ensure against stupidity
    
    //gl_Position = mvp * t_joint;
    gl_Position = mvp * vec4(in_Position.xyz, 1.0);;
    
    v_Color = in_Color;
    v_Tex = in_Tex;
}