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

struct platform_services
{
	bool (*GetFileSize)(const char*, u64*);
	bool (*ReadEntireFile)(const char*, u8*, u64);
};

struct vec2
{
	f32 X, Y;
};

struct vec3
{
	f32 X, Y, Z;
};

struct mat4
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

struct mesh
{
	GLuint VAO;
	u32 IndexCount;
};

struct model
{
	mesh Mesh;
	GLuint TexId;
	mat4 *Transform;
};

struct input_state
{
	u8 Up, Down, Left, Right, Back, Forward;
	u8 F1, F2;
};

struct game_state
{
	u32 Width;
	u32 Height;

	input_state Input;

	model Models[10];
	mesh Meshes[10];

	u8 *Memory;

	mat4 Proj;

	platform_services Services;
};

typedef void (__stdcall *pOdinFunc)(game_state *state);

struct vertex
{
	f32 x, y, z;
	f32 u, v;
	f32 nx, ny, nz;
};

#endif