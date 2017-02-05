#version 330 core

in vec2 fUV;
in vec3 fNormal;

// Light properties
uniform vec3 Light;	// Direction to the light
uniform vec3 LightColor;

// Material properties
uniform vec3 Albedo;
uniform vec3 Diff;
uniform vec3 Spec;

// Textures
uniform sampler2D AlbedoMap;

layout (location = 0) out vec4 fColor;

void main()
{
	float Kd = max(dot(Light, fNormal), 0.0);
	float Ks = 1.0;
	vec3 amb = vec3(0.1f);
	vec3 diff = vec3(1.0, 0.88, 0.74);
	vec3 spec = vec3(0.0, 0.0, 0.0);
	vec3 color = 0.6 * amb + Kd * diff + Ks * Spec;
	//color = vec3(fNormal);
	fColor = vec4(color, 1.0);
	fColor = texture(AlbedoMap, vec2(fUV.x, 1 - fUV.y));
	fColor.xyz *= Albedo * Kd + amb;
	fColor = vec4(1,1,1, 1.0);
}