#include <Odin/Shader.h>
#include <Util/File.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Load(const std::string& name)
{
	std::string vs = File::Read("../data/shaders/" + name + ".vs");
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	const char *c_str = vs.c_str();
	glShaderSource(vertex, 1, &c_str, nullptr);
	glCompileShader(vertex);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, 0, infoLog);
		std::cerr << infoLog << std::endl;
		return false;
	}

	std::string fs = File::Read("../data/shaders/" + name + ".fs");
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	c_str = fs.c_str();
	glShaderSource(fragment, 1, &c_str, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, 0, infoLog);
		std::cerr << infoLog << std::endl;
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, 0, infoLog);
		std::cerr << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::SetUniform(const char* name, const glm::mat4& mat)
{
	GLuint loc = glGetUniformLocation(program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}