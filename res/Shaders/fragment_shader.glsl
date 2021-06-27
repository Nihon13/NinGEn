#version 330 core

in vec3 v_Pos;
in vec3 v_Normal;
in vec2 v_TexCoord;

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

    FragColor = vec4(result, 1.0);
} 