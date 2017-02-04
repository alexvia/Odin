#include <windows.h>
#include <GL/GL.h>

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

int CALLBACK WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CmdLine, int CmdShow)
{
	WNDCLASS WindowClass {};
	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = Instance;
	//WindowClass.hIcon = ;
  	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  	WindowClass.lpszClassName = "ClassName";

	RegisterClass(&WindowClass);

	int Width = 1280;
	int Height = 720;
	RECT r = { 0, 0, Width, Height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	
	HWND Window = CreateWindow("ClassName",
 		"MyWindow", WS_OVERLAPPEDWINDOW,
 		CW_USEDEFAULT, CW_USEDEFAULT,
 		r.right - r.left, r.bottom - r.top,
 		0, 0, 0, 0);

	// Init OpenGL
	HDC DeviceContext = GetDC(Window);
	// ----- SetGLFormat(); -------------
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

	int indexPixelFormat = ChoosePixelFormat(DeviceContext, &pfd);
	SetPixelFormat(DeviceContext, indexPixelFormat, &pfd);
	// ----------------------------------
	HGLRC RenderingContext = wglCreateContext(DeviceContext);
	wglMakeCurrent(DeviceContext, RenderingContext);
	// ----- Resize(SCRWIDTH, SCRHEIGHT); ------
	glViewport(0, 0, Width, Height);
	// -----------------------------------------
	SwapBuffers(DeviceContext);
	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);

	bool running = true;
	MSG Msg;
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
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();//load identity matrix

		glBegin(GL_TRIANGLES);//start drawing triangles
		glVertex3f(-1.0f,-1.0f,0.0f);//triangle one first vertex
		glVertex3f(+1.0f,-1.0f,0.0f);//triangle one second vertex
		glVertex3f(+0.0f,+1.0f,0.0f);//triangle one third vertex
		glEnd();//end drawing of triangles

		// Swap buffers
		SwapBuffers(DeviceContext);
	}

	return 0;
}