#version 330 core

in vec3 fColor;

layout (location = 0) out vec4 color;

void main()
{
    color = /*vec4(fColor, 1.0f);*/vec4(1.0f, 0.27f, 0.0f, 1.0f);
}