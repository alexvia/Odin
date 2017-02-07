#include <math.h>


Vec3 operator-(const Vec3 &A, const Vec3 &B)
{
	return Vec3 { A.X - B.X, A.Y - B.Y, A.Z - B.Z };
}

float Dot(const Vec3 &A, const Vec3 &B)
{
	return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

Vec3 Cross(const Vec3 &A, const Vec3 &B)
{
	Vec3 V = 
	{ 
		A.Y * B.Z - B.Y * A.Z,
		A.Z * B.X - B.Z * A.X,
		A.X * B.Y - B.X * A.Y 
	};

	return V;
}

Vec3 Normalize(const Vec3 &A)
{
	float InvSqrt = 1.0f / sqrt(Dot(A, A));
	Vec3 V = { A.X * InvSqrt, A.Y * InvSqrt, A.Z * InvSqrt };
	return V;
}

Mat4 IdentityMat4()
{
	Mat4 M {};
	M._00 = M._11 = M._22 = M._33 = 1.0f;
	return M;
}

Mat4 operator*(const Mat4 &A, const Mat4 &B)
{
	Mat4 M {};
	
	M._00 = A._00*B._00 + A._01*B._10 + A._02*B._20 + A._03*B._30;
	M._01 = A._00*B._01 + A._01*B._11 + A._02*B._21 + A._03*B._31;
	M._02 = A._00*B._02 + A._01*B._12 + A._02*B._22 + A._03*B._32;
	M._03 = A._00*B._03 + A._01*B._13 + A._02*B._23 + A._03*B._33;

	M._10 = A._10*B._00 + A._11*B._10 + A._12*B._20 + A._13*B._30;
	M._11 = A._10*B._01 + A._11*B._11 + A._12*B._21 + A._13*B._31;
	M._12 = A._10*B._02 + A._11*B._12 + A._12*B._22 + A._13*B._32;
	M._13 = A._10*B._03 + A._11*B._13 + A._12*B._23 + A._13*B._33;

	M._20 = A._20*B._00 + A._21*B._10 + A._22*B._20 + A._23*B._30;
	M._21 = A._20*B._01 + A._21*B._11 + A._22*B._21 + A._23*B._31;
	M._22 = A._20*B._02 + A._21*B._12 + A._22*B._22 + A._23*B._32;
	M._23 = A._20*B._03 + A._21*B._13 + A._22*B._23 + A._23*B._33;

	M._30 = A._30*B._00 + A._31*B._10 + A._32*B._20 + A._33*B._30;
	M._31 = A._30*B._01 + A._31*B._11 + A._32*B._21 + A._33*B._31;
	M._32 = A._30*B._02 + A._31*B._12 + A._32*B._22 + A._33*B._32;
	M._33 = A._30*B._03 + A._31*B._13 + A._32*B._23 + A._33*B._33;

	return M;
}

Mat4 PerspectiveMat4(f32 FoV, f32 Aspect, f32 Near, f32 Far)
{
	Mat4 M {};

	f32 TanHalfFovY = tanf(FoV / 2.0f);
	M._00 = 1.0f / (Aspect * TanHalfFovY);
	M._11 = 1.0f / TanHalfFovY;
	M._32 = -1.0f; // Check
	M._22 = -(Far + Near) / (Far - Near);
	M._23 = -(2.0f * Far * Near) / (Far - Near); // Check

	return M;
}

Mat4 LookAtMat4(Vec3 &Eye, Vec3 &Center, Vec3 &Up)
{
	Mat4 M = IdentityMat4();

	Vec3 Front = Normalize(Center - Eye);
	Vec3 Side  = Normalize(Cross(Front, Up));
	Vec3 U     = Cross(Side, Front);

	M._00 = Side.X;
	M._01 = Side.Y;
	M._02 = Side.Z;
	M._10 = U.X;
	M._11 = U.Y;
	M._12 = U.Z;
	M._20 = -Front.X;
	M._21 = -Front.Y;
	M._22 = -Front.Z;
	M._03 = -Dot(Side, Eye);
	M._13 = -Dot(U, Eye);
	M._23 = +Dot(Front, Eye);

	return M;
}