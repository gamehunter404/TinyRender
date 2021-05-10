#include "Maths.h"

Vec2f Vec2_Add(Vec2f& a, Vec2f& b)
{
	return { a.x + b.x,a.y + b.y };
}

Vec2f Vec2_Sub(Vec2f& a, Vec2f& b)
{
	return {a.x - b.x,a.y-b.y};
}

Vec2f Vec2_Mul(Vec2f& a, float val)
{
	return {a.x*val,a.y*val};
}

Vec2Int Vec2i_Add(Vec2Int& a, Vec2Int& b)
{
	return {a.x+b.x,a.y+b.y};
}

Vec2Int Vec2i_Sub(Vec2Int& a, Vec2Int& b)
{
	return {a.x-b.x,a.y - b.y};
}

Mat3x3 RotationX(float radian)
{
	return {};
}



