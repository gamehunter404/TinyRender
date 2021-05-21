#ifndef __MATHS_H__
#define __MATHS_H__

#define PI 3.1415926535898
#define kError 1e-6
typedef struct { float x, y; } Vec2f;
typedef struct { int x, y,z;} Vec3Int;
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

	Vec3f operator-();
	void normalize();
	void mult(float val);
	void sub(const Vec3f v);
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

struct Mat3x3
{
public:
	Mat3x3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	Mat3x3();

	void mult(const Mat3x3&a);
	void mult(float val);
	void transpose();
	inline float* operator[](int index) const
	{
		return (float*)_data[index];
	}

private:
	float _data[3][3];
};


struct Mat4x4
{
public:
	Mat4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	Mat4x4();

	void mult(const Mat4x4&a);
	void mult(float val);
	void transpose();
	inline float* operator[](int index) const
	{
		return (float*)_data[index];
	}

private:
	float _data[4][4];
};

struct BoundingBox2D
{
	BoundingBox2D(Vec2f*v,int nums,float minX_,float maxX_,float minY_,float maxY_);
	BoundingBox2D(Vec2Int* v, int nums, float minX_, float maxX_, float minY_, float maxY_);
	float minX,maxX,maxY,minY;
};

void projVec(const Vec4f&res,Vec2f*tar);
void projVec(const Vec4f&res,Vec2Int*tar);
void projVec(const Vec4f &res,Vec3f*tar);

/*float relate function*/
float flt_Clamp(float v, float min, float max);
float flt_baryInterpolation(float a,float b,float c,const Vec3f&bary);
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
Vec3f vec3f_Add(const Vec3f&a,const Vec3f&b,const Vec3f&c);
Vec3f vec3f_Mul(const Vec3f&a ,float val);
Vec3f vec3f_Sub(const Vec3f& a, const Vec3f& b);
Vec3f vec3f_Cross(const Vec3f&a,const Vec3f&b);
Vec3f vec3f_baryInterpolation(const Vec3f&a,const Vec3f&b,const Vec3f&c,const Vec3f& bary);
Vec3f vec3f_Reflect(const Vec3f& nor,const Vec3f& inDir);
float vec3f_Dot(const Vec3f&a,const Vec3f&b);
Vec3f vec3f_Normalize(const Vec3f&a);


/*vector4 relate function*/
Vec4f vec4f_SetPoint(const Vec3f& a);
Vec4f vec4f_SetVec(const Vec3f& a);
Vec4f mat4x4_Mul_Vec4f(const Mat4x4&mat,const Vec4f& v);

/*mat3x3 relate funciton*/
Mat3x3 mat3x3_Identity();
float mat3x3_Det(const Mat3x3&m);

//mat4x4 relate function
Mat4x4 mat4x4_newByRow(const Vec3f&r1, const Vec3f& r2, const Vec3f& r3);
Mat4x4 mat4x4_newByColmun(const Vec3f& c1,const Vec3f&c2, const Vec3f& c3);
Mat4x4 mat4x4_Mul(const Mat4x4&a,const Mat4x4&b);
Mat4x4 mat4x4_Transpose(const Mat4x4& m);
Mat4x4 mat4x4_IdentityMat();
Mat4x4 mat4x4_Inverse(const Mat4x4&m);
float mat4x4_Det(const Mat4x4&m);
Mat4x4 mat4x4_Adjoint(const Mat4x4&m);
float mat4x4_Cofactor(const Mat4x4&m,int row,int col);
float mat4x4_Minor(const Mat4x4&m,int row,int col);
Mat3x3 mat4x4_Minimize(const Mat4x4&m,int row,int col);
bool mat4x4_IsEqual(const Mat4x4&a,const Mat4x4&b);


Mat4x4 getScaleMat(const Vec3f& scale);
Mat4x4 getTranslateMat(const Vec3f&tran);
Mat4x4 getRotationXMat(float radians);
Mat4x4 getRotationYMat(float radians);
Mat4x4 getRotationZMat(float radians);


Mat4x4 getModelMat(const Vec3f& scale,const Vec3f& rotation ,const Vec3f& tran);
Mat4x4 getInverseModelMat(const Vec3f& scale, const Vec3f& rotation, const Vec3f& tran);
Mat4x4 getViewMat(Vec3f pos, Vec3f target,Vec3f up);
Mat4x4 getProjectionMat(const Device&device);
Mat4x4 getPerspectiiveMat(float near,float far);
Mat4x4 getOrthogonalMat(const Vec4f& canonicalSize,float near,float far);
Mat4x4 getViewPortMat(float nx,float ny);


struct Color
{

public:
	Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_);
	Color(unsigned char r_, unsigned char g_, unsigned char b_);

	unsigned int Data() const;
	Color operator*(float val) const;
	void set(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_);
	void set(unsigned char r_, unsigned char g_, unsigned char b_);
	unsigned char r();
	unsigned char g();
	unsigned char b();

private:
	unsigned int _data;
	unsigned char _a, _r, _g, _b;
};

#endif // !__MATHS_H__




