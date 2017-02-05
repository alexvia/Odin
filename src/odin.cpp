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

struct Vertex
{
	f32 x, y, z;
	f32 u, v;
	f32 nx, ny, nz;
};

// --------------------------------------------------------------------------
// Export functions called from the platform layer

extern "C" __declspec(dllexport) void __stdcall Init(Game_State *State)
{
	u64 Size;
	State->Services.GetFileSize("Shaders/uber.vs", &Size);
	State->Services.ReadEntireFile("Shaders/uber.vs", State->Memory, Size);
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

	State->Services.GetFileSize("Shaders/uber.fs", &Size);
	State->Services.ReadEntireFile("Shaders/uber.fs", State->Memory, Size);
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

	f32 Vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+0.0f, +1.0f, 0.0f
	};

	u32 Indices[] =
	{
		0, 1, 2
	};

	State->Services.GetFileSize("Models/bonelord_2side_part1.msh", &Size);
	State->Services.ReadEntireFile("Models/bonelord_2side_part1.msh", State->Memory, Size);

	u8 *magic = State->Memory;
	u32 version = *(u32*)(State->Memory + 4);
	u32 vCount = *(u32*)(State->Memory + 8);
	Vertex *vertices = (Vertex*)(State->Memory + 12);
	u32 iCount = *(u32*)(State->Memory + 12 + (vCount) * sizeof(Vertex));
	u32 *indices = (u32*)(State->Memory + 12 + (vCount) * sizeof(Vertex) + 4);

	GLuint VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(f32)));
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(u32), indices, GL_STATIC_DRAW);

	f32 ProjView[] = 
	{
		1.358f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.414f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, -1.0f,
		0.0f, -24.14f, 24.805f, 25.0f
	};

	f32 Model[] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glUniformMatrix4fv(glGetUniformLocation(Program, "ProjView"), 1, GL_FALSE, ProjView);
	glUniformMatrix4fv(glGetUniformLocation(Program, "Model"), 1, GL_FALSE, Model);

	// ----- Resize(SCRWIDTH, SCRHEIGHT); ------
	glViewport(0, 0, State->Width, State->Height);
	// glMatrixMode(GL_PROJECTION);	// Remove this
	// glLoadIdentity();				// Remove this
	// gluPerspective(45.0f, Width / Height, 0.2f, 255.0f);	// Remove this
	// glMatrixMode(GL_MODELVIEW);		// Remove this
	// -----------------------------------------

	LoadShaders("simple");
}

extern "C" __declspec(dllexport) void __stdcall UpdateAndRender(Game_State *State)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 1590, GL_UNSIGNED_INT, 0);
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