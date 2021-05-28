#include "Maths.h"
#include"Device.h"
#include<iostream>

void projVec(const Vec4f& res, Vec2f* tar)
{
	if (tar == nullptr)
		return;

	tar->x = res.x;
	tar->y = res.y;
}

void projVec(const Vec4f& res, Vec2Int* tar)
{
	if (tar == nullptr) 
		return;

	tar->x = res.x;
	tar->y = res.y;
}

void projVec(const Vec4f& res, Vec3f* tar)
{
	if (tar == nullptr) 
		return;

	tar->x = res.x;
	tar->y = res.y;
	tar->z = res.z;
}

Vec2Int projVec(const Vec4f& res)
{
	return Vec2Int(res.x,res.y);
}

float flt_Clamp(float v, float min, float max)
{
	if (v < min) v = min;
	else if (v > max) v = max;
	return v;
}

float flt_baryInterpolation(float a, float b, float c, const Vec3f& bary)
{
	return a*bary.x+b*bary.y+c*bary.z;
}

float angleToRadians(float angle)
{
	return angle*PI/180;
}

void flt_Printf(const char* name, float val)
{
	printf("%s : \n",name);
	printf("%12f\n",val);
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

Vec3f vec3f_Add(const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
	return Vec3f(a.x+b.x+c.x,a.y+b.y+c.y,a.z+b.z+c.z);
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

Vec3f vec3f_baryInterpolation(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& bary)
{

	return Vec3f(a.x*bary.x+b.x*bary.y+c.x*bary.z,a.y*bary.x+b.y*bary.y+c.y*bary.z,a.z*bary.x+b.z*bary.y+c.z*bary.z);
}

Vec3f vec3f_Reflect(const Vec3f& nor,const Vec3f& inDir)
{
	Vec3f res = nor;
	res.mult(2.0f*vec3f_Dot(nor, inDir));
	res.sub(inDir);
	res.normalize();
	return res;
}

float vec3f_Dot(const Vec3f& a, const Vec3f& b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vec3f vec3f_Normalize(const Vec3f& a)
{
	float mag = a.magnitude()+kError;
	return Vec3f(a.x/mag,a.y/mag,a.z/mag);
}

void vec3f_Printf(const char* name, const Vec3f& v)
{
	printf("Vec3f %s :\n",name);
	printf(" %12f, %12f, %12f\n",v.x,v.y,v.z);
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

Vec4f mat4x4_Mul_Vec4f(const Mat4x4& mat, const Vec4f& v)
{
	return Vec4f(mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w,
				 mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w,
				 mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w,
				 mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3] * v.w);
}

void vec4f_Printf(const char* name, const Vec4f& v)
{
	printf("Vec4f %s :\n", name);
	printf(" %12f, %12f, %12f, %12f\n", v.x, v.y, v.z,v.w);
}

Mat3x3 mat3x3_Identity()
{
	return Mat3x3
		(1, 0, 0,
		0, 1, 0,
		0, 0, 1);
}

Mat3x3 mat3x3_Mul(const Mat3x3& a, const Mat3x3& b)
{
	Mat3x3 res;
	float tmp[3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tmp[j] = a[i][0]*b[0][j]+
				a[i][1]*b[1][j]+a[i][2]*b[2][j];
		}

		res[i][0] = tmp[0];
		res[i][1] = tmp[1];
		res[i][2] = tmp[2];

	}

	return res;
}

Mat3x3 mat3x3_MakeCol(const Vec3f& c0, const Vec3f& c1, const Vec3f& c2)
{
	return Mat3x3(c0.x, c1.x, c2.x,
				c0.y, c1.y, c2.y,
				c0.z, c1.z, c2.z);
}

Mat3x3 mat3x3_MakeRow(const Vec3f& r0, const Vec3f& r1, const Vec3f& r2)
{
	return Mat3x3(r0.x, r0.y, r0.z,
				r1.x, r1.y, r1.z,
				r2.x, r2.y, r2.z);
}

float mat3x3_Det(const Mat3x3& m)
{
	float det = 0;

	det += m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	det += -m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	det += m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	return det;
}

Mat3x3 mat3x3_Inverse(const Mat3x3& m)
{
	Mat3x3 res = mat3x3_Adjoint(m);
	float det = 0;

	for (int j = 0; j < 3; j++)
		det += m[0][j] * res[0][j];

	res.transpose();
	res.mult(1.0f/(det+kError));
	return res;
}

Mat3x3 mat3x3_Adjoint(const Mat3x3& m)
{
	Mat3x3 res;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			res[i][j] = mat3x3_Cofactor(m,i,j);
		}
	}

	return res;
}

float mat3x3_Cofactor(const Mat3x3& m, int row, int col)
{
	float tmp[2][2];
	float sig = (row + col) % 2 == 0 ? 1 : -1;
	int r = 0, c = 0;

	for (int i = 0; i < 3; i++)
	{
		if (i == row) 
			continue;

		for (int j = 0,c = 0; j < 3; j++)
		{
			if (j == col)
				continue;

			tmp[r][c] = m[i][j];
			c++;
		}
		r++;
	}

	return sig*(tmp[0][0]*tmp[1][1] - tmp[0][1]*tmp[1][0]);
}

Vec3f mat3x3_Mul_Vec3f(const Mat3x3& m, const Vec3f& v)
{
	return Vec3f(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
				 m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
				 m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
}


Mat4x4 mat4x4_newByRow(const Vec3f& r1, const Vec3f& r2, const Vec3f& r3)
{
	return Mat4x4(r1.x,r1.y,r1.z,0,
				  r2.x,r2.y,r2.z,0,
				  r3.x,r3.y,r3.z,0,
				  0,0,0,1);
}

Mat4x4 mat4x4_newByColmun(const Vec3f& c1, const Vec3f& c2, const Vec3f& c3)
{
	return Mat4x4(c1.x,c2.x,c3.x,0,
				  c1.y,c2.y,c3.y,0,
				  c1.z,c2.z,c3.z,0,
				  0,   0,   0,   1);
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

Mat4x4 mat4x4_Inverse(const Mat4x4& m)
{
	Mat4x4 res = mat4x4_Adjoint(m);

	float det = 0;
	for (int j = 0; j < 4; j++)
		det += res[0][j]*m[0][j];

	res.transpose();
	res.mult(1.0f/(det+kError));

	return res;
}

void mat4x4_Printf(const char* name, const Mat4x4& m)
{
	printf("%s = \n",name);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("    %12f",m[i][j]);
		}

		printf("\n");
	}
}

float mat4x4_Det(const Mat4x4& m)
{
	float det = 0;
	for (int j = 0; j < 4; j++)
	{
		det += m[0][j] * mat4x4_Cofactor(m,0,j);
	}

	return det;
}

Mat4x4 mat4x4_Adjoint(const Mat4x4& m)
{
	Mat4x4 res;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i][j] = mat4x4_Cofactor(m,i,j);
		}
	}

	return res;
}

float mat4x4_Cofactor(const Mat4x4& m, int row, int col)
{
	float sig = (row + col) % 2 == 0?1.0f:-1.0f;
	return sig*mat4x4_Minor(m,row,col);
}

float mat4x4_Minor(const Mat4x4& m, int row, int col)
{
	Mat3x3 tmp = mat4x4_Minimize(m,row,col);
	return mat3x3_Det(tmp);
}

Mat3x3 mat4x4_Minimize(const Mat4x4& m, int row, int col)
{
	int r = 0, c = 0;
	Mat3x3 res;

	for (int i = 0; i < 4; i++)
	{
		if (i == row) continue;

		for (int j = 0,c = 0; j < 4; j++)
		{
			if (j == col) continue;

			res[r][c] = m[i][j];
			c++;
		}
		r++;
	}

	return res;
}

bool mat4x4_IsEqual(const Mat4x4& a, const Mat4x4& b)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float dif = a[i][j] - b[i][j];

			if (std::abs(dif) > kError) 
				return false;

		}
	}


	return true;
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
	/*Mat4x4 res = getScaleMat(scale);
	Mat4x4 translate = getTranslateMat(tran);
	res = mat4x4_Mul(getRotationXMat(angleToRadians(rotation.x)), res);
	res = mat4x4_Mul(getRotationYMat(angleToRadians(rotation.y)), res);
	res = mat4x4_Mul(getRotationZMat(angleToRadians(rotation.z)), res);
	res = mat4x4_Mul(getTranslateMat(tran),res);*/

	Mat4x4 res = getTranslateMat(tran);
	res.mult(getRotationZMat(angleToRadians(rotation.z)));
	res.mult(getRotationYMat(angleToRadians(rotation.y)));
	res.mult(getRotationXMat(angleToRadians(rotation.x)));
	res.mult(getTranslateMat(tran));
	res.mult(getScaleMat(scale));

	return res;
}

Mat4x4 getInverseModelMat(const Vec3f& scale, const Vec3f& rotation, const Vec3f& tran)
{
	Mat4x4 res = mat4x4_IdentityMat();
	res[3][0] = -tran.x;
	res[3][1] = -tran.y;
	res[3][2] = -tran.z;

	res.mult(getRotationZMat(angleToRadians(rotation.z)));
	res.mult(getRotationYMat(angleToRadians(rotation.y)));
	res.mult(getRotationXMat(angleToRadians(rotation.x)));

	res.mult(getScaleMat(Vec3f(1/scale.x,1/scale.y,1/scale.z)));

	return res;
}

Mat4x4 getViewMat( Vec3f pos,  Vec3f target,  Vec3f up)
{
	Vec3f to = vec3f_Sub(pos, target);
	to.normalize();
	up.normalize();

	Vec3f right = vec3f_Cross(up, to);
	up = vec3f_Cross(to, right);
	up.normalize();
	/*Mat4x4 res = mat4x4_IdentityMat();
	Mat4x4 mat = mat4x4_newByRow(right, up, to);
	Mat4x4 tran = getTranslateMat(-pos);
	res = mat4x4_Mul(mat, tran);*/
	
	Mat4x4 res = mat4x4_newByRow(right, up, to);
	res.mult(getTranslateMat(-pos));

	return res;
}

Mat4x4 getProjectionMat(const Device& device)
{
	/*;
	Mat4x4 res = mat4x4_IdentityMat();

	if (device.projectionType == ProjectionType::PROJECTION_PERSPECTIVE) {

		res = mat4x4_Mul(getPerspectiiveMat(device.nearPlane,device.farPlane),res);
	}

	res = mat4x4_Mul(getOrthogonalMat(canonicalSize,device.nearPlane,device.farPlane),res);
	res = mat4x4_Mul(getViewPortMat(device.width,device.height),res);*/

	Vec4f canonicalSize = Vec4f(device.l, device.r, device.b, device.t);
	Mat4x4 res = getOrthogonalMat(canonicalSize, device.nearPlane, device.farPlane);

	if (device.projectionType == ProjectionType::PROJECTION_PERSPECTIVE) {
		res.mult(getPerspectiiveMat(device.nearPlane, device.farPlane));
	}

	return res;
}

Mat4x4 getPerspectiiveMat(float n, float f)
{

	n = std::abs(n);
	f = std::abs(f);

	return Mat4x4(n,0,0,0,
				   0,n,0,0,
				   0,0,f + n,f*n,
				   0,0,-1,0);
}

Mat4x4 getOrthogonalMat(const Vec4f& canonicalSize, float n, float f)
{
	const float& l = canonicalSize.x;
	const float& r = canonicalSize.y;
	const float& b = canonicalSize.z;
	const float& t = canonicalSize.w;

	n = std::abs(n);
	f = std::abs(f);

	return Mat4x4(2/(r-l),0,0,-(r+l)/(r - l),
					0,2/(t-b),0,-(t+b)/(t - b),
					0,0,-2/(f- n),-(f+n)/(f-n),
					0,0,0,1);
}

Mat4x4 getViewPortMat(float nx, float ny)
{
	return Mat4x4(nx/2,0,0,nx/2,
				  0,-ny/2,0,ny/2,
				  0,0,1.0f/2.0f,1.0f/2.0f,
				  0,0,0,1);
}

Vec4f perspectiveDivision(const Vec4f&v)
{
	return Vec4f(v.x/v.w,v.y/v.w,v.z/v.w,v.w);
}

void perspectiveDivision(Vec4f* v)
{
	float div = 1.0f / (v->w+kError);

	v->x *= div;
	v->y *= div;
	v->z *= div;
	v->w = 1.0f;
}


Vec3f Vec3f::operator-() const
{
	return Vec3f(-x,-y,-z);
}

Vec3f Vec3f::operator*(float val) const
{
	return Vec3f(x*val,y*val,z*val);
}

void Vec3f::normalize()
{
	float mag = magnitude()+kError;
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

void Vec3f::mult(float val)
{
	x *= val;
	y *= val;
	z *= val;
}

void Vec3f::sub(const Vec3f v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
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

	_data[0][0] = m00, _data[0][1] = m01, _data[0][2] = m02, _data[0][3] = m03;
	_data[1][0] = m10, _data[1][1] = m11, _data[1][2] = m12, _data[1][3] = m13;
	_data[2][0] = m20, _data[2][1] = m21, _data[2][2] = m22, _data[2][3] = m23;
	_data[3][0] = m30, _data[3][1] = m31, _data[3][2] = m32, _data[3][3] = m33;
}

Mat4x4::Mat4x4()
{
	memset(_data,0,sizeof(float)*16);
	_data[0][0] = _data[1][1] = _data[2][2] = _data[3][3] = 1;
}

Mat4x4 Mat4x4::operator*(const Mat4x4& mat)
{
	Mat4x4 res;

	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			res[i][j] = _data[i][0] * mat[0][j] +
				_data[i][1] * mat[1][j] +
				_data[i][2] * mat[2][j] +
				_data[i][3] * mat[3][j];
		}
	}

	return res;
}

void Mat4x4::mult(const Mat4x4& a)
{
	float tmp[4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp[j] = _data[i][0] * a[0][j] + 
					 _data[i][1] * a[1][j] + 
					 _data[i][2] * a[2][j] + 
					 _data[i][3] * a[3][j];
		}

		_data[i][0] = tmp[0];
		_data[i][1] = tmp[1];
		_data[i][2] = tmp[2];
		_data[i][3] = tmp[3];

	}
}

void Mat4x4::mult(float val)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			_data[i][j] *= val;
	}
}

void Mat4x4::transpose()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			std::swap(_data[i][j],_data[j][i]);
		}
	}
}
BoundingBox2D::BoundingBox2D(Vec2f* v, int nums, float minX_, float maxX_, float minY_, float maxY_) :minX(0),maxX(0),minY(0),maxY(0)
{
	if (v == nullptr) return;

	minX = minY = std::numeric_limits<float>::max();
	maxX = maxY = std::numeric_limits<float>::min();

	for (int i = 0; i < nums; i++)
	{
		minX = std::min(minX,v[i].x);
		minY = std::min(minY,v[i].y);
		maxX = std::max(maxX,v[i].x);
		maxY = std::max(maxY,v[i].y);
	}

	minX = std::max(minX,minX_);
	minY = std::max(minY, minY_);
	maxX = std::min(maxX,maxX_);
	maxY = std::min(maxY,maxY_);
}

BoundingBox2D::BoundingBox2D(Vec2Int* v, int nums, float minX_, float maxX_, float minY_, float maxY_) :minX(0), minY(0),maxX(0),maxY(0)
{
	if (v == nullptr) return;

	minX = minY = std::numeric_limits<float>::max();
	maxX = maxY = std::numeric_limits<float>::min();

	for (int i = 0; i < nums; i++)
	{
		minX = std::min(minX, static_cast<float>(v[i].x));
		minY = std::min(minY, static_cast<float>(v[i].y));
		maxX = std::max(maxX, static_cast<float>(v[i].x));
		maxY = std::max(maxY, static_cast<float>(v[i].y));
	}

	minX = std::max(minX, minX_);
	minY = std::max(minY, minY_);
	maxX = std::min(maxX, maxX_);
	maxY = std::min(maxY, maxY_);
}

Color::Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_) :_r(r_), _g(g_), _b(b_), _a(a_)
{
	float div = (float)a_ / 255;
	r_ = r_ * div;
	g_ = g_ * div;
	b_ = b_ * div;

	_data = (0xff000000) | (r_ << 16) | (g_ << 8) | (b_);
}

Color::Color(unsigned char r_, unsigned char g_, unsigned char b_) :_r(r_), _g(g_), _b(b_), _a(255)
{
	_data = (0xff000000) | (_r << 16) | (_g << 8) | (_b);
}

void Color::set(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_)
{
	float div = (float)a_ / 255;
	r_ = r_ * div;
	g_ = g_ * div;
	b_ = b_ * div;

	_data = (0xff000000) | (r_ << 16) | (g_ << 8) | (b_);
}

void Color::set(unsigned char r_, unsigned char g_, unsigned char b_)
{
	_r = r_;
	_g = g_;
	_b = b_;
	_data = (0xff000000) | (_r << 16) | (_g << 8) | (_b);
}

unsigned char Color::r()
{
	return _r;
}

unsigned char Color::g()
{
	return _g;
}

unsigned char Color::b()
{
	return _b;
}

unsigned int Color::Data() const
{
	return _data;

}

Color Color::operator*(float val) const
{
	val = flt_Clamp(val, 0.f, 1.f);
	return Color(_r * val, _g * val, _b * val);
}

Mat3x3::Mat3x3(float m00, float m01, float m02, 
	float m10, float m11, float m12, 
	float m20, float m21, float m22)
{
	_data[0][0] = m00;
	_data[0][1] = m01;
	_data[0][2] = m02;

	_data[1][0] = m10;
	_data[1][1] = m11;
	_data[1][2] = m12;

	_data[2][0] = m20;
	_data[2][1] = m21;
	_data[2][2] = m22;

}

Mat3x3::Mat3x3()
{
	memset(_data, 0x00, sizeof(float) * 9);
}

void Mat3x3::mult(const Mat3x3& a)
{
	float tmp[3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tmp[j] = _data[i][0] * a[0][j] +
					 _data[i][1] * a[1][j] +
					 _data[i][2] * a[2][j];
		}

		_data[i][0] = tmp[0];
		_data[i][1] = tmp[1];
		_data[i][2] = tmp[2];

	}
}

void Mat3x3::mult(float val)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			_data[i][j] *= val;
	}
}

void Mat3x3::transpose()
{
	std::swap(_data[0][1],_data[1][0]);
	std::swap(_data[0][2], _data[2][0]);
	std::swap(_data[1][2], _data[2][1]);

}
