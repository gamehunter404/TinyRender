#ifndef __MATHS_H__
#define __MATHS_H__


typedef struct { float x, y; } Vec2f;
typedef struct { int x, y,z;} Vec3Int;
typedef struct { float m[3][3]; } Mat3x3;
typedef struct { float m[4][4]; } Mat4x4;


struct Vec2Int
{
public:
	Vec2Int(int x_, int y_) :x(x_), y(y_) {}
	Vec2Int() :x(0), y(0) {}

public:
	int x, y;
};


struct Vec3f
{
public:
	Vec3f() :x(0), y(0), z(0) {}
	Vec3f(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}


	void Normalize();
	float Magnitude();

public:
	float x, y, z;
};



/*float relate function*/
float Clamp(float v, float min, float max);



/*vector2 relate function*/
Vec2f Vec2f_Add(const Vec2f&a,const Vec2f&b);
Vec2f Vec2f_Sub(const Vec2f&a,const Vec2f&b);
Vec2f Vec2f_Mul(const Vec2f&a, float val);


/*vector2int relate funtion*/
Vec2Int Vec2i_Add(const Vec2Int& a,const Vec2Int& b);
Vec2Int Vec2i_Sub(const Vec2Int& a,const Vec2Int& b);


/*vector3 relate function*/
Vec3f Vec3f_Add(const Vec3f&a,const Vec3f&b);
Vec3f Vec3f_Sub(const Vec3f& a, const Vec3f& b);
Vec3f Vec3f_Cross(const Vec3f&a,const Vec3f&b);
float Vec3f_Dot(const Vec3f&a,const Vec3f&b);



//m3x3 relate function
Mat3x3 RotationX(float radian);
Mat3x3 RotationY(float radian);
Mat3x3 RotationZ(float radian);


#endif // !__MATHS_H__




