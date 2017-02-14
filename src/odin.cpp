#include "odin.h"
#include "odin_math.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

static platform_services Services;
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

GLuint LoadShader(char *Name)
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

mesh LoadMesh(const char *Path)
{
	u64 Size;
	Services.GetFileSize(Path, &Size);
	Services.ReadEntireFile(Path, Memory, Size);

	u8 *magic = Memory;
	u32 version = *(u32*)(Memory + 4);
	u32 vCount = *(u32*)(Memory + 8);
	vertex *vertices = (vertex*)(Memory + 12);
	u32 iCount = *(u32*)(Memory + 12 + (vCount) * sizeof(vertex));
	u32 *indices = (u32*)(Memory + 12 + (vCount) * sizeof(vertex) + 4);

	GLuint VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vCount * sizeof(vertex), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(5 * sizeof(f32)));
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(u32), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	mesh m {};
	m.VAO = VAO;
	m.IndexCount = iCount;
	return m;
}

mesh LoadQuad()
{
	f32 vertices[] = 
	{
		-1.0f, -1.0f,	0.0f, 0.0f,
		+1.0f, -1.0f,	1.0f, 0.0f,
		+1.0f, +1.0f,	1.0f, 1.0f,
		-1.0f, +1.0f,	0.0f, 1.0f
	};

	u32 indices[] = { 0, 1, 2, 2, 3, 0 };

	GLuint VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	mesh m {};
	m.VAO = VAO;
	m.IndexCount = 6;
	return m;
}

GLuint LoadTexture(const char *Path)
{
	int x, y, n;
	u8 *data = stbi_load(Path, &x, &y, &n, 0);
	GLuint TexId;
	glGenTextures(1, &TexId);
	glBindTexture(GL_TEXTURE_2D, TexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return TexId;
}

GLuint LoadFontTexture(const char *Path, char C)
{
	u64 Size;
	Services.GetFileSize("c:/windows/fonts/arialbd.ttf", &Size);
	u8 *FontText = new u8[Size];
	Services.ReadEntireFile("c:/windows/fonts/arialbd.ttf", FontText, Size);
	stbtt_fontinfo Font;
   	int Width, Height, XOffset, YOffset;
   	stbtt_InitFont(&Font, FontText, stbtt_GetFontOffsetForIndex(FontText, 0));
   	u8 *Bitmap = stbtt_GetCodepointBitmap(&Font, 0, stbtt_ScaleForPixelHeight(&Font, 128), 
   		'b', &Width, &Height, &XOffset, &YOffset);

   	GLuint TexId;
	glGenTextures(1, &TexId);
	glBindTexture(GL_TEXTURE_2D, TexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, Bitmap);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbtt_FreeBitmap(Bitmap, 0);

	return TexId;
}

// --------------------------------------------------------------------------
// Export functions called from the platform layer
GLuint MeshShader, TextShader;
extern "C" __declspec(dllexport) void __stdcall Init(game_state *State)
{
	Services = State->Services;
	Memory = State->Memory;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, State->Width, State->Height);

	State->Proj = PerspectiveMat4(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

	MeshShader = LoadShader("uber");
	TextShader = LoadShader("text");

	State->Models[0].Mesh = LoadMesh("Models/bonelord_part1.msh");
	State->Models[0].TexId = LoadTexture("Textures/lord/bonelord_alpha.png");

	State->Models[1].Mesh = LoadMesh("Models/bonelord_part2_part1.msh");
	State->Models[1].TexId = State->Models[0].TexId;
	
	State->Models[2].Mesh = LoadMesh("Models/bonelord_part2_part2.msh");
	State->Models[2].TexId = State->Models[0].TexId;
	
	State->Models[3].Mesh = LoadMesh("Models/bonelord_2side_part1.msh");
	State->Models[3].TexId = LoadTexture("Textures/lord/bonelord_2side_alpha.png");
	
	State->Models[4].Mesh = LoadMesh("Models/bonelord_2side_part2.msh");
	State->Models[4].TexId = State->Models[3].TexId;

	State->Models[5].Mesh = LoadQuad();
	State->Models[5].TexId = LoadFontTexture("c:/windows/fonts/arialbd.ttf", 'b');	
}

extern "C" __declspec(dllexport) void __stdcall UpdateAndRender(game_state *State)
{
	if(State->Input.F1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if(State->Input.F2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	static vec3 Pos { 0.0f, 10.0f, 25.0f };
	static vec3 Target { 0.0f, 10.0f, 0.0f };
	float Speed = 0.1f;
	if (State->Input.Up)
	{
		Pos.Y += Speed;
		Target.Y += Speed;
	}
	if (State->Input.Down)
	{
		Pos.Y += -Speed;
		Target.Y += -Speed;
	}
	if (State->Input.Left)
	{
		Pos.X += -Speed;
		Target.X += -Speed;
	}
	if (State->Input.Right)
	{
		Pos.X += Speed;
		Target.X += Speed;
	}
	if (State->Input.Back)
	{
		Pos.Z += Speed;
		Target.Z += Speed;
	}
	if (State->Input.Forward)
	{
		Pos.Z += -Speed;
		Target.Z += -Speed;
	}

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(MeshShader);

	vec3 Up { 0.0f, 1.0f, 0.0f };
	mat4 View = LookAtMat4(Pos, Target, Up);
	glUniformMatrix4fv(glGetUniformLocation(MeshShader, "View"), 1, GL_FALSE, View.data);

	glUniformMatrix4fv(glGetUniformLocation(MeshShader, "Proj"), 1, GL_FALSE, State->Proj.data);

	glUniformMatrix4fv(glGetUniformLocation(MeshShader, "Model"), 1, GL_FALSE, TranslationMat4(0.0f, -20.0f, -100.0f).data);
	for(int i = 0; i < 10; i++)
	{
		if(State->Models[i].Mesh.VAO)
		{
			if (i == 5)
			{
				glUseProgram(TextShader);
				glUniformMatrix4fv(glGetUniformLocation(TextShader, "Proj"), 1, GL_FALSE, (State->Proj * View).data);
			}

			if (State->Models[i].Transform)
			{
				glUniformMatrix4fv(glGetUniformLocation(MeshShader, "Model"), 1, GL_FALSE, State->Models[i].Transform->data);
			}

			glBindVertexArray(State->Models[i].Mesh.VAO);
			glBindTexture(GL_TEXTURE_2D, State->Models[i].TexId);
			glDrawElements(GL_TRIANGLES, State->Models[i].Mesh.IndexCount, GL_UNSIGNED_INT, 0);
		}
	}
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