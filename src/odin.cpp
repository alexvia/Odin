#include "odin.h"
#include "odin_math.cpp"

static Platform_Services Services;
static u8 *Memory;

// --------------------------------------------------------------------------
// Util functions (Split in multiple files)

GLuint GetShader(const char *FullPath, GLenum type)
{
	u64 Size;
	Services.GetFileSize(FullPath, &Size);
	Services.ReadEntireFile(FullPath, Memory, Size);
	Memory[Size] = 0;
	GLuint Shader = glCreateShader(type);
	const GLchar *source = (const GLchar *)Memory;
	glShaderSource(Shader, 1, &source, 0);
	glCompileShader(Shader);

	GLint Success;
	GLchar InfoLog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Shader, 512, 0, InfoLog);
		OutputDebugString(InfoLog);
	}

	return Shader;
}

GLuint LoadShaders(char *Name)
{
	// TODO: Implement better string manipulation
	char FullPath[MAX_PATH] { "Shaders/" };
	char *cursor = FullPath;
	while(*cursor != 0) cursor++;
	for(char *i = Name; *i; i++) *cursor++ = *i;
	*cursor++ = '.';
	*cursor++ = 'v';
	*cursor = 's';
	
	GLuint VShader = GetShader(FullPath, GL_VERTEX_SHADER);

	cursor--;
	*cursor = 'f';
	GLuint FShader = GetShader(FullPath, GL_FRAGMENT_SHADER);

	GLuint Program = glCreateProgram();
	glAttachShader(Program, VShader);
	glAttachShader(Program, FShader);
	glLinkProgram(Program);

	GLint Success;
	GLchar InfoLog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(Program, 512, 0, InfoLog);
		OutputDebugString(InfoLog);
	}

	glDeleteShader(VShader);
	glDeleteShader(FShader);

	return Program;
}

Mesh LoadMesh(const char *Path)
{
	u64 Size;
	Services.GetFileSize("Models/bonelord_2side_part1.msh", &Size);
	Services.ReadEntireFile("Models/bonelord_2side_part1.msh", Memory, Size);

	u8 *magic = Memory;
	u32 version = *(u32*)(Memory + 4);
	u32 vCount = *(u32*)(Memory + 8);
	Vertex *vertices = (Vertex*)(Memory + 12);
	u32 iCount = *(u32*)(Memory + 12 + (vCount) * sizeof(Vertex));
	u32 *indices = (u32*)(Memory + 12 + (vCount) * sizeof(Vertex) + 4);

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
	glBindVertexArray(0);

	Mesh m {};
	m.VAO = VAO;
	m.IndexCount = iCount;
	return m;
}

// --------------------------------------------------------------------------
// Export functions called from the platform layer

extern "C" __declspec(dllexport) void __stdcall Init(Game_State *State)
{
	Services = State->Services;
	Memory = State->Memory;

	GLuint Program = LoadShaders("uber");
	glUseProgram(Program);

	State->Meshes[0] = LoadMesh("Models/bonelord_2side_part1.msh");

	Mat4 Proj = PerspectiveMat4(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	Mat4 View = LookAtMat4(Vec3 {0.0f, 10.0f, 25.0f}, Vec3 {0.0f, 10.0f, 0.0f}, Vec3 {0.0f, 1.0f, 0.0f});
	Mat4 ProjView = Proj * View;
	Mat4 Model = IdentityMat4();

	glUniformMatrix4fv(glGetUniformLocation(Program, "ProjView"), 1, GL_FALSE, ProjView.data);
	glUniformMatrix4fv(glGetUniformLocation(Program, "Model"), 1, GL_FALSE, Model.data);

	glViewport(0, 0, State->Width, State->Height);
}

extern "C" __declspec(dllexport) void __stdcall UpdateAndRender(Game_State *State)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	Vec3 Pos { 0.0f, 10.0f, 25.0f };
	Vec3 Target { 0.0f, 10.0f, 0.0f };
	Vec3 Up { 0.0f, 1.0f, 0.0f };

	glBindVertexArray(State->Meshes[0].VAO);
	glDrawElements(GL_TRIANGLES, State->Meshes[0].IndexCount, GL_UNSIGNED_INT, 0);
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