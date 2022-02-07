#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ModelMatrix = mat4(1.0f);
uniform mat4 u_ViewMatrix = mat4(1.0f);
uniform mat4 u_ProjectionMatrix = mat4(1.0f);

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Pos, 1.0f);
}