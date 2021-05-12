#include "Maths.h"
#include<iostream>

float Clamp(float v, float min, float max)
{
	if (v < min) v = min;
	else if (v > max) v = max;
	return v;
}

Vec2f Vec2f_Add(const Vec2f& a,const Vec2f& b)
{
	return { a.x + b.x,a.y + b.y };
}

Vec2f Vec2f_Sub(const Vec2f& a,const Vec2f& b)
{
	return {a.x - b.x,a.y-b.y};
}

Vec2f Vec2f_Mul(const Vec2f& a, float val)
{
	return {a.x*val,a.y*val};
}

Vec2Int Vec2i_Add(const Vec2Int& a,const Vec2Int& b)
{
	return {a.x+b.x,a.y+b.y};
}

Vec2Int Vec2i_Sub(const Vec2Int& a,const Vec2Int& b)
{
	return {a.x-b.x,a.y - b.y};
}

Vec3f Vec3f_Add(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vec3f Vec3f_Sub(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vec3f Vec3f_Cross(const Vec3f& a, const Vec3f& b)
{
	return {a.y*b.z - b.y*a.z,
			a.z*b.x - a.x*b.z,
			a.x*b.y - b.x*a.y};
}

float Vec3f_Dot(const Vec3f& a, const Vec3f& b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

Mat3x3 RotationX(float radian)
{
	return {};
}

void Vec3f::Normalize()
{
	float magnitude = Magnitude();
	x = x / magnitude;
	y = y / magnitude;
	z = z / magnitude;
}

float Vec3f::Magnitude()
{
	return std::sqrt(x * x + y * y+z*z);
}
