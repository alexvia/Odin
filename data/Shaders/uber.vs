#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vUV;
layout (location = 2) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 ProjView;
uniform mat4 Model;

out vec2 fUV;
out vec3 fNormal;

void main()
{
	fUV = vUV;
	fNormal = vNormal;
	gl_Position = ProjView * Model * vPos;
}