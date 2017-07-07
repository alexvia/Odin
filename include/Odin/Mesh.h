#pragma once

#include <glad/glad.h>

#include <vector>

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<float>& vertices);
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	void Init(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();
	void Draw();	

private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint indexCount = 0;
};