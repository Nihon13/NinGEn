#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Bitangent;
layout (location = 4) in vec3 a_Tangent;
layout (location = 5) in ivec3 a_BoneIDs;
layout (location = 6) in vec3 a_Weights;

uniform mat4 u_ModelMatrix = mat4(1.0f);
uniform mat4 u_ViewMatrix = mat4(1.0f);
uniform mat4 u_ProjectionMatrix = mat4(1.0f);

const int MAX_BONES = 30;
const int MAX_BONE_INFLUENCE = 3;

uniform mat4 u_FinalBonesMatrices[MAX_BONES];

out vec3 v_Pos;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    vec4 totalLocalPos = vec4(0.0f);

    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (a_BoneIDs[i] == -1)
        {
            continue;
        }

        vec4 localPos = u_FinalBonesMatrices[a_BoneIDs[i]] * vec4(a_Pos, 1.0f);
        totalLocalPos += localPos * a_Weights[i];
    }

    // gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * totalLocalPos;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Pos, 1.0f);

    v_Pos = vec3(u_ModelMatrix * vec4(a_Pos, 1.0));
    v_Normal = mat3(u_ModelMatrix) * a_Normal;
    v_TexCoord = a_TexCoord;
}