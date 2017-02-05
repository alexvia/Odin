#include "odin.h"

// --------------------------------------------------------------------------
// Util functions (Split in multiple files)

void LoadShaders(const char *Path)
{
	char data[10];
	int n;
	//int x = ReadEntireFile(path, data, &n);
	
	/*
	ReadFileContents(path);
	std::string specificPath = "../Assets/Shaders/" + std::string(path);
	if (type == GL_VERTEX_SHADER)
		specificPath += ".vs";
	else if (type == GL_FRAGMENT_SHADER)
		specificPath += ".fs";

	std::ifstream fileStream(specificPath);
	std::string sourceString(std::istreambuf_iterator<char>(fileStream), {});
	const GLchar *source = sourceString.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, 0, infoLog);
		std::cout << "Failed to compile " << specificPath.c_str() << "\n" 
			<< infoLog << std::endl;
	}

	return shader;*/
}

// --------------------------------------------------------------------------
// Export functions called from the platform layer

extern "C" __declspec(dllexport) void __stdcall Init(Game_State *State)
{
	u64 Size;
	State->Services.GetFileSize("Shaders/simple.vs", &Size);
	State->Services.ReadEntireFile("Shaders/simple.vs", State->Memory, Size);
	GLuint VShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *source = (const GLchar *)State->Memory;
	glShaderSource(VShader, 1, &source, 0);
	glCompileShader(VShader);

	GLint Success;
	GLchar InfoLog[512];
	glGetShaderiv(VShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(VShader, 512, 0, InfoLog);
		OutputDebugString(InfoLog);
	}

	State->Services.GetFileSize("Shaders/simple.fs", &Size);
	State->Services.ReadEntireFile("Shaders/simple.fs", State->Memory, Size);
	State->Memory[Size] = 0;
	GLuint FShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = (const GLchar *const)State->Memory;
	glShaderSource(FShader, 1, &source, 0);
	glCompileShader(FShader);

	glGetShaderiv(FShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FShader, 512, 0, InfoLog);
		OutputDebugString(InfoLog);
	}

	GLuint Program = glCreateProgram();
	glAttachShader(Program, VShader);
	glAttachShader(Program, FShader);
	glLinkProgram(Program);

	glGetProgramiv(Program, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(Program, 512, 0, InfoLog);
		OutputDebugString(InfoLog);
	}

	glDeleteShader(VShader);
	glDeleteShader(FShader);

	glUseProgram(Program);

	float Vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+0.0f, +1.0f, 0.0f
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ----- Resize(SCRWIDTH, SCRHEIGHT); ------
	glViewport(0, 0, State->Width, State->Height);
	// glMatrixMode(GL_PROJECTION);	// Remove this
	// glLoadIdentity();				// Remove this
	// gluPerspective(45.0f, Width / Height, 0.2f, 255.0f);	// Remove this
	// glMatrixMode(GL_MODELVIEW);		// Remove this
	// -----------------------------------------

	LoadShaders("");
}

extern "C" __declspec(dllexport) void __stdcall UpdateAndRender(Game_State *State)
{
	//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 9);
}

// --------------------------------------------------------------------------
// Entry Point

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	// Load OpenGL extensions when loading the dll
	if(Reason == DLL_PROCESS_ATTACH)
	{
		if(!gladLoadGL())
		{
			return false;
		}
	}

	return true;
}

// --------------------------------------------------------------------------