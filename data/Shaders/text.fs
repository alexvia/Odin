#version 330 core

in vec2 fUV;

uniform sampler2D textureMap;
//uniform vec3 color;

layout (location = 0) out vec4 fColor;

void main()
{
	//fColor.xyz = color;	
	fColor.w = texture(textureMap, vec2(fUV.x, 1 - fUV.y)).r;
	//fColor = vec4(1);
}