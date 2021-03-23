#version 330 core

in vec3 pos;
out vec4 FragColor;

void main()
{
    if (pos.x < 0)
        FragColor = vec4(pos, 1.0f);
    else
        FragColor = vec4(-pos, 1.0f);
}