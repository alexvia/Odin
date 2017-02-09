#version 330 core

layout (location = 0) in vec4 vCoord;

uniform mat4 Proj;

out vec2 fUV;

void main()
{
	fUV = vCoord.zw;
	gl_Position = Proj * vec4(vCoord.xy, 0, 1);
}