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
	State->services.GetFileSize("Shaders/simple.vs", &Size);
	State->services.ReadEntireFile("Shaders/simple.vs", State->Memory, Size);
	GLuint Shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *const source = (const GLchar *const)State->Memory;
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

	glBegin(GL_TRIANGLES);//start drawing triangles
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);//triangle one first vertex
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(+1.0f,-1.0f,0.0f);//triangle one second vertex
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(+0.0f,+1.0f,0.0f);//triangle one third vertex
	glEnd();//end drawing of triangles
}

// --------------------------------------------------------------------------
// Entry Point

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	// Load OpenGL extension when loading the dll
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