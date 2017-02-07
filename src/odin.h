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

#define Kilobytes(x) (1024 * (x))
#define Megabytes(x) (1024 * 1024 * (x))
#define Gigabytes(x) (1024 * 1024 * 1024 (x))

#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

struct Platform_Services
{
	bool (*GetFileSize)(const char*, u64*);
	bool (*ReadEntireFile)(const char*, u8*, u64);
};

struct Vec3
{
	f32 X, Y, Z;
};

struct Mat4
{
	union
	{
		struct
		{	// Column-Major order
			f32 _00, _10, _20, _30;	// First Column
			f32 _01, _11, _21, _31;	// Second Column
			f32 _02, _12, _22, _32;	// Third Column
			f32 _03, _13, _23, _33;	// Fourth Column
		};
		struct
		{
			f32 C0[4], C1[4], C2[4], C3[4];
		};
		f32 M[4][4];
		f32 data[16];
	};
};

struct Mesh
{
	GLuint VAO;
	u32 IndexCount;
};

struct Model
{
	Mesh *mesh;
	GLuint TexId;
	Mat4 *Transform;
};

struct Input_State
{
	u8 Up, Down, Left, Right, Back, Forward;
	u8 F1, F2;
};

struct Game_State
{
	u32 Width;
	u32 Height;

	Input_State Input;

	Mesh Meshes[10];

	u8 *Memory;

	Platform_Services Services;
};

typedef void (__stdcall *pOdinFunc)(Game_State *state);

struct Vertex
{
	f32 x, y, z;
	f32 u, v;
	f32 nx, ny, nz;
};

#endif