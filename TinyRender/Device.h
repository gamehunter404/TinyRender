#pragma once
#include"tgaImage.h"
#include"Maths.h"
#include<vector>

struct Mat4x4;
struct Vec3f;

//三角形填充算法
enum TriangleFillAlgorithm
{
	TRIFILL_SWEEPING = 0,
	TRIFILL_EDGEEQUATION,
};

//投影类型
enum ProjectionType
{
	PROJECTION_PERSPECTIVE,//透视投影
	PROJECTION_ORTHOGONAL,//正交投影
};


struct Device
{
public:
	unsigned int** frameBuf;
	float** zBuf;
	int width;
	int height;

	Vec3f light_Dir =Vec3f(0,0,-1);//光的方向

	float screenRatio;
	float nearPlane = 1.f;//近平面
	float farPlane = 1000.;//远平面
	float fov = 55;//垂直视域
	float l, r, t, b;
	float light_intensity;

	ProjectionType projectionType = ProjectionType::PROJECTION_PERSPECTIVE;//投影类型
	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;

};