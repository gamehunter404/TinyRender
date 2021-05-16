#ifndef __MATHS_H__
#define __MATHS_H__

#define PI 3.1415926535898
#define kError 1e-6
typedef struct { float x, y; } Vec2f;
typedef struct { int x, y,z;} Vec3Int;
typedef struct { float m[3][3]; } Mat3x3;
struct Device;

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

	Vec3f operator- () const;
	Vec3f operator*(float val) const;
	void normalize();
	float magnitude() const;

public:
	float x, y, z;

};

struct Vec4f:public Vec3f
{
public:
	Vec4f(float x_, float y_, float z_, float w_):Vec3f(x_,y_,z_),w(w_) {}
	Vec4f() :w(0),Vec3f(0,0,0){}

public:
	float w;
};


struct Mat4x4
{
public:
	Mat4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	Mat4x4();
	Mat4x4(const Vec3f& c1, const Vec3f& c2, const Vec3f& c3, const Vec3f& c4);

	void transpose();
	inline float* operator[](int index) const
	{
		return (float*)m[index];
	}

public:
	float m[4][4];
};


/*float relate function*/
float flt_Clamp(float v, float min, float max);
float angleToRadians(float angle);


/*vector2 relate function*/
Vec2f vec2f_Add(const Vec2f&a,const Vec2f&b);
Vec2f vec2f_Sub(const Vec2f&a,const Vec2f&b);
Vec2f vec2f_Mul(const Vec2f&a, float val);


/*vector2int relate funtion*/
Vec2Int vec2i_Add(const Vec2Int& a,const Vec2Int& b);
Vec2Int vec2i_Sub(const Vec2Int& a,const Vec2Int& b);


/*vector3 relate function*/
Vec3f vec3f_Zero();
Vec3f vec3f_Add(const Vec3f&a,const Vec3f&b);
Vec3f vec3f_Mul(const Vec3f&a ,float val);
Vec3f vec3f_Sub(const Vec3f& a, const Vec3f& b);
Vec3f vec3f_Cross(const Vec3f&a,const Vec3f&b);
float vec3f_Dot(const Vec3f&a,const Vec3f&b);
Vec3f vec3f_Normalize(const Vec3f&a);


/*vector4 relate function*/
Vec4f vec4f_SetPoint(const Vec3f& a);
Vec4f vec4f_SetVec(const Vec3f& a);
Vec4f vec4f_Mul(const Mat4x4&mat,const Vec4f& v);


//mat4x4 relate function
void mat4x4_Mul(const Mat4x4& a, const Mat4x4& b,Mat4x4* res);
Mat4x4 mat4x4_Mul(const Mat4x4&a,const Mat4x4&b);
Mat4x4 mat4x4_Transpose(const Mat4x4& m);
Mat4x4 mat4x4_IdentityMat();


Mat4x4 getScaleMat(const Vec3f& scale);//获取缩放矩阵
Mat4x4 getTranslateMat(const Vec3f&tran);//获取平移矩阵
Mat4x4 getRotationXMat(float radians);
Mat4x4 getRotationYMat(float radians);
Mat4x4 getRotationZMat(float radians);


Mat4x4 getModelMat(const Vec3f& scale,const Vec3f& rotation ,const Vec3f& tran);//获取模型变换矩阵
Mat4x4 getViewMat(Vec3f pos, Vec3f target,Vec3f up);//获取视口变换矩阵
Mat4x4 getProjectionMat(const Device&device);
Mat4x4 getPerspectiiveMat(float near,float far);//获取投影矩阵
Mat4x4 getOrthogonalMat(const Vec4f& canonicalSize,float near,float far);//获取正交矩阵
Mat4x4 getViewPortMat(float nx,float ny);//获取视口矩阵

struct Color
{

public:
	Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_):r(r_),g(g_),b(b_),a(a_)
	{

		float div = (float)a_ / 255;
		r_ = r_*div;
		g_ = g_*div;
		b_ = b_*div;

		data = (0xff000000) | (r_ << 16) | (g_ << 8) | (b_);

	}

	Color(unsigned char r_, unsigned char g_, unsigned char b_) :r(r_), g(g_), b(b_), a(255)
	{
		data = (0xff000000) | (r <<16) | (g <<8) | (b);
	}
	
	unsigned int Data() const
	{
		return data;
	}

	Color operator*(float val)
	{
		val = flt_Clamp(val, 0.f, 1.f);
		return Color(r * val, g * val, b * val);
	}

private:
	unsigned int data;
	unsigned char a, r, g, b;
};

#endif // !__MATHS_H__




