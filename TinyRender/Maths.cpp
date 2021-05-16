#include "Maths.h"
#include"Device.h"
#include<iostream>

float flt_Clamp(float v, float min, float max)
{
	if (v < min) v = min;
	else if (v > max) v = max;
	return v;
}

float angleToRadians(float angle)
{
	return angle*PI/180;
}

Vec2f vec2f_Add(const Vec2f& a,const Vec2f& b)
{
	return { a.x + b.x,a.y + b.y };
}

Vec2f vec2f_Sub(const Vec2f& a,const Vec2f& b)
{
	return {a.x - b.x,a.y-b.y};
}

Vec2f vec2f_Mul(const Vec2f& a, float val)
{
	return {a.x*val,a.y*val};
}

Vec2Int vec2i_Add(const Vec2Int& a,const Vec2Int& b)
{
	return {a.x+b.x,a.y+b.y};
}

Vec2Int vec2i_Sub(const Vec2Int& a,const Vec2Int& b)
{
	return {a.x-b.x,a.y - b.y};
}

Vec3f vec3f_Zero()
{
	return Vec3f(0,0,0);
}

Vec3f vec3f_Add(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vec3f vec3f_Mul(const Vec3f& a, float val)
{
	return Vec3f(a.x*val,a.y*val,a.z*val);
}

Vec3f vec3f_Sub(const Vec3f& a, const Vec3f& b)
{
	return Vec3f(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vec3f vec3f_Cross(const Vec3f& a, const Vec3f& b)
{
	return {a.y*b.z - b.y*a.z,
			a.z*b.x - a.x*b.z,
			a.x*b.y - b.x*a.y};
}

float vec3f_Dot(const Vec3f& a, const Vec3f& b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vec3f vec3f_Normalize(const Vec3f& a)
{
	//warning Ð¡ÐÄ³ýÁã´íÎó
	float mag = a.magnitude()+kError;
	return Vec3f(a.x/mag,a.y/mag,a.z/mag);
}

Vec3f mul_Point(const Vec3f& v, const Mat4x4& mt)
{
	
	return Vec3f(v.x * mt[0][0] + v.y * mt[0][1] + v.z * mt[0][2] + mt[0][3],
				 v.x * mt[1][0] + v.y * mt[1][1] + v.z * mt[1][2] + mt[1][3],
				 v.x * mt[2][0] + v.y * mt[2][1] + v.z * mt[2][2] + mt[2][3]);
}

Vec3f mul_Vec(const Vec3f& v, const Mat4x4& mt)
{
	return Vec3f(v.x * mt[0][0] + v.y * mt[0][1] + v.z * mt[0][2],
				 v.x * mt[1][0] + v.y * mt[1][1] + v.z * mt[1][2],
				 v.x * mt[2][0] + v.y * mt[2][1] + v.z * mt[2][2]);
}

Vec4f vec4f_SetPoint(const Vec3f& a)
{
	return Vec4f(a.x,a.y,a.z,1);
}

Vec4f vec4f_SetVec(const Vec3f& a)
{
	return Vec4f(a.x,a.y,a.z,0);
}

Vec4f vec4f_Mul(const Mat4x4& mat, const Vec4f& v)
{
	return Vec4f(mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w,
				 mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w,
				 mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w,
				 mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3] * v.w);
}

void mat4x4_Mul(const Mat4x4& a, const Mat4x4& b, Mat4x4* res)
{
	Mat4x4& tmp = *res;
	float sum = 0;

	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			tmp[i][j] = 0;

			for (int k = 0; k < 4; k++)
			{
				tmp[i][j] += a[i][k] * b[k][j];
			}
		}
	}


}

Mat4x4 mat4x4_Mul(const Mat4x4& a, const Mat4x4& b)
{
	Mat4x4 res;

	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			res[i][j] = a[i][0] * b[0][j] +
						a[i][1] * b[1][j] + 
						a[i][2] * b[2][j] + 
						a[i][3] * b[3][j];
		}
	}

	return res;

}

Mat4x4 mat4x4_Transpose(const Mat4x4& m)
{
	Mat4x4 res;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i][j] = m[j][i];
		}
	}
	return res;
}

Mat4x4 mat4x4_IdentityMat()
{
	return Mat4x4(1,0,0,0,
				  0,1,0,0,
				  0,0,1,0,
				  0,0,0,1);
}

Mat4x4 getScaleMat(const Vec3f& scale)
{
	return Mat4x4(scale.x,0,0,0,
				  0,scale.y,0,0,
				  0,0,scale.z,0,
				  0,0,0,1);
}

Mat4x4 getTranslateMat(const Vec3f& tran)
{
	return Mat4x4(1,0,0,tran.x,
				 0,1,0,tran.y,
				0,0,1,tran.z,
				0,0,0,1);
}

Mat4x4 getRotationXMat(float radians)
{
	float c = std::cos(radians);
	float s = std::sin(radians);
	return Mat4x4(1, 0,0,0,
				  0, c,-s,0,
				  0, s,c,0,
				  0, 0,0,1);
}

Mat4x4 getRotationYMat(float radians)
{
	float c = std::cos(radians);
	float s = std::sin(radians);
	return Mat4x4( c, 0,  s, 0,
				   0, 1, 0, 0,
				   -s, 0,  c, 0,
				   0, 0,  0, 1);
}

Mat4x4 getRotationZMat(float radians)
{
	float c = std::cos(radians);
	float s = std::sin(radians);
	return Mat4x4(c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Mat4x4 getModelMat(const Vec3f& scale, const Vec3f& rotation, const Vec3f& tran)
{
	Mat4x4 res = getScaleMat(scale);
	Mat4x4 translate = getTranslateMat(tran);
	res = mat4x4_Mul(getRotationXMat(angleToRadians(rotation.x)), res);
	res = mat4x4_Mul(getRotationYMat(angleToRadians(rotation.y)), res);
	res = mat4x4_Mul(getRotationZMat(angleToRadians(rotation.z)), res);
	res = mat4x4_Mul(getTranslateMat(tran),res);

	return res;
}

Mat4x4 getViewMat( Vec3f pos,  Vec3f target,  Vec3f up)
{
	Vec3f to = vec3f_Sub(pos, target);
	to.normalize();
	up.normalize();

	Vec3f right = vec3f_Cross(up, to);
	up = vec3f_Cross(to, right);

	Mat4x4 res;
	Mat4x4 mat = Mat4x4(right, up, to, vec3f_Zero());
	mat.transpose();
	Mat4x4 tran = getTranslateMat(-pos);
	mat4x4_Mul(mat, tran, &res);
	return res;
}

Mat4x4 getProjectionMat(const Device& device)
{
	Mat4x4 res = mat4x4_IdentityMat();
	Vec4f canonicalSize = Vec4f(device.l,device.r,device.t,device.b);

	if (device.projectionType == ProjectionType::PROJECTION_PERSPECTIVE) {

		res = mat4x4_Mul(getPerspectiiveMat(device.nearPlane,device.farPlane),res);
	}

	res = mat4x4_Mul(getOrthogonalMat(canonicalSize,device.nearPlane,device.farPlane),res);
	res = mat4x4_Mul(getViewPortMat(device.width,device.height),res);

	return res;
}

Mat4x4 getPerspectiiveMat(float n, float f)
{
	return Mat4x4(-n,0,0,0,
				   0,-n,0,0,
				   0,0,f+n,-f*n,
				   0,0,1,0);
}

Mat4x4 getOrthogonalMat(const Vec4f& canonicalSize, float n, float f)
{
	const float& l = canonicalSize.x;
	const float& r = canonicalSize.y;
	const float& t = canonicalSize.z;
	const float& b = canonicalSize.w;

	return Mat4x4(2/(r-l),0,0,(r+l)/(l-r),
					0,2/(t-b),0,(t+b)/(b-t),
					0,0,2/(n-f),(f+n)/(f-n),
					0,0,0,1);
}

Mat4x4 getViewPortMat(float nx, float ny)
{
	return Mat4x4(nx/2,0,0,nx/2,
				  0,-ny/2,0,ny/2,
				  0,0,1,0,
				  0,0,0,1);
}


Vec3f Vec3f::operator-() const
{
	return Vec3f(-x, -y, -z);
}

Vec3f Vec3f::operator*(float val) const
{
	return Vec3f(x*val,y*val,z*val);
}

void Vec3f::normalize()
{
	//warning Ð¡ÐÄ³ýÁã´íÎó
	float mag = magnitude()+kError;
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

float Vec3f::magnitude() const
{
	return std::sqrt(x * x + y * y+z*z);
}

Mat4x4::Mat4x4( float m00, float m01, float m02, float m03, 
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23, 
				float m30, float m31, float m32, float m33)
{

	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
}

Mat4x4::Mat4x4()
{
	memset(m,0,sizeof(float)*16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
}

Mat4x4::Mat4x4(const Vec3f& c1, const Vec3f& c2, const Vec3f& c3, const Vec3f& c4)
{
	m[0][0] = c1.x;
	m[1][0] = c1.y;
	m[2][0] = c1.z;
	m[3][0] = 0;

	m[0][1] = c2.x;
	m[1][1] = c2.y;
	m[2][1] = c2.z;
	m[3][1] = 0;
	
	m[0][2] = c3.x;
	m[1][2] = c3.y;
	m[2][2] = c3.z;
	m[3][2] = 0;


	m[0][3] = c4.x;
	m[1][3] = c4.y;
	m[2][3] = c4.z;
	m[3][3] = 1;
}

void Mat4x4::transpose()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			std::swap(m[i][j],m[j][i]);
		}
	}
}