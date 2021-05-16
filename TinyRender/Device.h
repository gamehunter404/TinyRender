#pragma once
#include"tgaImage.h"
#include"Maths.h"

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
	float screenRatio;
	float nearPlane = 0.1f;//近平面
	float farPlane = 10000.f;//远平面
	float fov = 60;//垂直视域
	float l, r, t, b;

	Mat4x4 vpMat;
	Mat4x4 viewPortMat;
	Mat4x4 viewMat;
	Mat4x4 modelMat;
	Mat4x4 inverseModelMat;

	TGAImage* texture;
	int texWidth;
	int texHeight;

	Vec3f light_Dir = Vec3f(0, 0, -1);//光的方向

	ProjectionType projectionType = ProjectionType::PROJECTION_PERSPECTIVE;//投影类型
	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;

};