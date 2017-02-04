#include <windows.h>

LRESULT CALLBACK WindowProc(HWND Window,
	UINT Msg, WPARAM WParam, LPARAM LParam)
{
	return DefWindowProc(Window, Msg, WParam, LParam);
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
  	//WindowClass.hCursor = ;
  	WindowClass.lpszClassName = "ClassName";

	RegisterClass(&WindowClass);

	HWND Window = CreateWindow("ClassName",
 		"MyWindow", WS_OVERLAPPEDWINDOW,
 		CW_USEDEFAULT, CW_USEDEFAULT,
 		CW_USEDEFAULT, CW_USEDEFAULT,
 		0, 0, 0, 0);

	ShowWindow(Window, CmdShow);

	MSG Msg = {};
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}	

	return 0;
}