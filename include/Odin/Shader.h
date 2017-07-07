#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Shader
{
public:
	Shader();
	~Shader();
	bool Load(const std::string& name);
	void Use();
	void SetUniform(const char* name, const glm::mat4& mat);

private:
	GLuint program = 0;
};