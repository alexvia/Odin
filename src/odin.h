#ifndef _ODIN_H
#define _ODIN_H

#include "glad/glad.h"

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;


#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

struct Platform_Services
{
	bool (*GetFileSize)(const char*, u64*);
	bool (*ReadEntireFile)(const char*, u8*, u64);
};

struct Game_State
{
	u32 Width;
	u32 Height;

	u8 *Memory;

	Platform_Services Services;
};

typedef void (__stdcall *pOdinFunc)(Game_State *state);

#endif