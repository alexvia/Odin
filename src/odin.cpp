#include "odin.h"

extern "C" __declspec(dllexport) void __stdcall Init(Game_State *state)
{
	gladLoadGL();
	
	// ----- Resize(SCRWIDTH, SCRHEIGHT); ------
	glViewport(0, 0, state->Width, state->Height);
	// glMatrixMode(GL_PROJECTION);	// Remove this
	// glLoadIdentity();				// Remove this
	// gluPerspective(45.0f, Width / Height, 0.2f, 255.0f);	// Remove this
	// glMatrixMode(GL_MODELVIEW);		// Remove this
	// -----------------------------------------
}

extern "C" __declspec(dllexport) void __stdcall UpdateAndRender(Game_State *state)
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