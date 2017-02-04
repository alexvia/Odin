#include "glad/glad.h"

#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

struct Game_State
{
	int Width;
	int Height;
};

typedef void (__stdcall *pOdinFunc)(Game_State *state);