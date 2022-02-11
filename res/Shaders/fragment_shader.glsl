#version 330 core

in vec3 v_Pos;
in vec3 v_Normal;
in vec2 v_TexCoord;

flat in ivec3 v_BoneIDs;
in vec3 v_Weights;
uniform int gDisplayBoneIndex;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * texture(u_Texture, v_TexCoord).rgb;

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(vec3(0.0, 900.0, 0.0) - v_Pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 lightDiffuse = vec3(0.7, 0.7, 0.7);
    vec3 diffuse = lightDiffuse * diff * texture(u_Texture, v_TexCoord).rgb;

    vec3 result = ambient + diffuse;

    // FragColor = vec4(result, 1.0);

    bool found = false;

    for (int i = 0; i < 3; i++) {
        if (v_BoneIDs[i] == gDisplayBoneIndex) {
            if (v_Weights[i] >= 0.7)
            {
                FragColor = vec4(1.0, 0.0, 0.0, 0.0) * v_Weights[i];
            }
            else if (v_Weights[i] >= 0.4 && v_Weights[i] <= 0.6)
            {
              FragColor = vec4(0.0, 1.0, 0.0, 0.0) * v_Weights[i];
            }
            else if (v_Weights[i] >= 0.1)
            {
              FragColor = vec4(1.0, 1.0, 0.0, 0.0) * v_Weights[i];
            }

            found = true;
            break;
        }
    }

    if(!found)
    {
        FragColor = vec4(result, 1.0);
    }
} 