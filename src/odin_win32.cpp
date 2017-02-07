#include "odin.h"

// ----------------------------------------------------------------
// Platform Services

bool GetFileSize(const char *Path, u64 *Size)
{
	HANDLE FileHandle = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, 0, 0);

	if(FileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	LARGE_INTEGER FileSize = {};
	if(!GetFileSizeEx(FileHandle, &FileSize))
	{
		return false;
	}

	*Size = FileSize.QuadPart;

	if(!CloseHandle(FileHandle))
	{
		return false;
	}

	return true;
}

bool ReadEntireFile(const char *Path, u8 *Data, u64 BytesToRead)
{
	HANDLE FileHandle = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, 0, 0);

	if(FileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD BytesRead = 0;
	if(!ReadFile(FileHandle, Data, (u32)BytesToRead, &BytesRead, 0))
	{
		return false;
	}

	if(!CloseHandle(FileHandle))
	{
		return false;
	}

	return true;
}



// ----------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND Window,
	UINT Msg, WPARAM WParam, LPARAM LParam)
{
	switch(Msg)
	{
		case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
        	return DefWindowProc(Window, Msg, WParam, LParam);
	}

	return 0;
}

void HandleInput(MSG *Msg, Input_State *Input)
{
	switch(Msg->message)
	{
		case WM_KEYDOWN:
		case WM_KEYUP:
			Input->Up = (Msg->wParam == 'W') && !(Msg->lParam >> 31);
			Input->Down = (Msg->wParam == 'S') && !(Msg->lParam >> 31);
			Input->Left = (Msg->wParam == 'A') && !(Msg->lParam >> 31);
			Input->Right = (Msg->wParam == 'D') && !(Msg->lParam >> 31);
			Input->Back = (Msg->wParam == 'Q') && !(Msg->lParam >> 31);
			Input->Forward = (Msg->wParam == 'E') && !(Msg->lParam >> 31);

			Input->F1 = (Msg->wParam == VK_F1) && !(Msg->lParam >> 31);
			Input->F2 = (Msg->wParam == VK_F2) && !(Msg->lParam >> 31);
	}
}

void InitOpenGL(HWND Window, HDC *DeviceContext, HGLRC *RenderingContext)
{
	*DeviceContext = GetDC(Window);
	Assert(*DeviceContext != 0);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 
		32,
		0,0,0,0,0,0,0,0,0,0,0,0,0, // useless parameters
		16,
		0,0,0,0,0,0,0
	};

	int indexPixelFormat = ChoosePixelFormat(*DeviceContext, &pfd);
	SetPixelFormat(*DeviceContext, indexPixelFormat, &pfd);

	*RenderingContext = wglCreateContext(*DeviceContext);
	Assert(*RenderingContext != 0);
	Assert(wglMakeCurrent(*DeviceContext, *RenderingContext));
}

int CALLBACK WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CmdLine, int CmdShow)
{
	// Register Window class
	WNDCLASS WindowClass {};
	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = Instance;
	//WindowClass.hIcon = LoadIcon(NULL, IDC_ICON);;
  	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  	WindowClass.lpszClassName = "ClassName";
	RegisterClass(&WindowClass);

	if(!SetCurrentDirectory("../data"))
	{
		return -1;
	}

	// Needed to set the dimensions in pixels
	if(!SetProcessDPIAware())
	{
		return -1;
	}

	// Compute Window dimensions
	// TODO: Load this from a config file
	u32 Width = 1280;
	u32 Height = 720;
	RECT r = { 0, 0, (LONG)Width, (LONG)Height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	
	// Create Window
	HWND Window = CreateWindow(WindowClass.lpszClassName,
 		"Odin", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
 		r.right - r.left, r.bottom - r.top,
 		0, 0, Instance, 0);

	// Init OpenGL
	HDC DeviceContext = 0;
	HGLRC RenderingContext = 0;
	InitOpenGL(Window, &DeviceContext, &RenderingContext);

	HINSTANCE GameDLL = LoadLibrary("odin.dll");
	pOdinFunc OdinInit = (pOdinFunc)GetProcAddress(GameDLL, "Init");
	pOdinFunc OdinUpdateAndRender = (pOdinFunc)GetProcAddress(GameDLL, "UpdateAndRender");
	Game_State State {};
	State.Width = Width;
	State.Height = Height;
	State.Memory = (u8*)VirtualAlloc(0, Megabytes(1), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

	// Configure the platform services
	Platform_Services Services {};
	Services.GetFileSize = GetFileSize;
	Services.ReadEntireFile = ReadEntireFile;

	State.Services = Services;

	OdinInit(&State);

	SwapBuffers(DeviceContext);
	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);

	bool running = true;
	MSG Msg;

	// TODO: Fix frame rate
	while(running)
	{
		if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		{
			if(Msg.message == WM_QUIT || (Msg.message == WM_KEYDOWN && Msg.wParam == VK_ESCAPE))
			{
				running = false;
			}

			HandleInput(&Msg, &State.Input);

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// Update and Render
		OdinUpdateAndRender(&State);

		// Swap buffers
		SwapBuffers(DeviceContext);
	}

	// Cleanup
	wglMakeCurrent(DeviceContext, 0);
	wglDeleteContext(RenderingContext);

	return 0;
}