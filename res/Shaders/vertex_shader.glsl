#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_ModelMatrix = mat4(1.0f);
uniform mat4 u_ViewMatrix = mat4(1.0f);
uniform mat4 u_ProjectionMatrix = mat4(1.0f);

out vec3 v_Pos;
out vec2 v_TexCoord;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Pos, 1.0);
    v_Pos = vec3(u_ProjectionMatrix * vec4(a_Pos, 1.0));
    v_TexCoord = a_TexCoord;
}