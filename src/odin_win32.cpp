#include "odin.h"

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
		0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
		16,
		0,0,0,0,0,0,0
	};

	int indexPixelFormat = ChoosePixelFormat(*DeviceContext, &pfd);
	SetPixelFormat(*DeviceContext, indexPixelFormat, &pfd);

	*RenderingContext = wglCreateContext(*DeviceContext);
	Assert(*RenderingContext != 0);
	Assert(wglMakeCurrent(*DeviceContext, *RenderingContext));

	// Load OpenGL Extensions
	/*if (!gladLoadGL())
	{
		MessageBox(0, "gladLoadGL() failed", "Failed to load OpenGL", 0);
	}*/
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
	//WindowClass.hIcon = ;
  	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  	WindowClass.lpszClassName = "ClassName";
	RegisterClass(&WindowClass);

	// Compute Window dimensions
	// TODO: Load this from a config file
	int Width = 1280;
	int Height = 720;
	RECT r = { 0, 0, Width, Height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);

	// Create Window
	HWND Window = CreateWindow(WindowClass.lpszClassName,
 		"Odin", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
 		r.right - r.left, r.bottom - r.top,
 		0, 0, 0, 0);

	// Init OpenGL
	HDC DeviceContext = 0;
	HGLRC RenderingContext = 0;
	InitOpenGL(Window, &DeviceContext, &RenderingContext);

	HINSTANCE GameDLL = LoadLibrary("odin.dll");
	pOdinFunc OdinInit = (pOdinFunc)GetProcAddress(GameDLL, "Init");
	pOdinFunc OdinUpdateAndRender = (pOdinFunc)GetProcAddress(GameDLL, "UpdateAndRender");
	Game_State state {};
	state.Width = Width;
	state.Height = Height;
	//state.Memory = VirtualAlloc(..);
	OdinInit(&state);

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
			if(Msg.message == WM_QUIT)
			{
				running = false;
			}

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		// Update and Render
		OdinUpdateAndRender(&state);

		// Swap buffers
		SwapBuffers(DeviceContext);
	}

	// Cleanup
	wglMakeCurrent(DeviceContext, 0);
	wglDeleteContext(RenderingContext);

	return 0;
}