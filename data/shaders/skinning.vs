#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in ivec3 jointIDs;
layout (location = 3) in vec3 weights;

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

uniform mat4 mvp;
uniform mat4 joints[MAX_JOINTS];

out vec3 fColor;

void main()
{
	fColor = color;

	vec4 totalLocalPos = vec4(0.0f);
	for(int i = 0; i < MAX_WEIGHTS; i++)
	{
		vec4 localPos = joints[jointIDs[i]] * vec4(pos, 1.0f);
		totalLocalPos += localPos * weights[i];
	}

	gl_Position = mvp * totalLocalPos;
}