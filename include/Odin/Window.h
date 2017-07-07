#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	~Window();
	bool Create(const char* title = "Odin Window");
	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
	inline int Width() { return width; }
	inline int Height() { return height; }

private:
	int width = 1280;
	int height = 720;
	GLFWwindow *window = nullptr;
};