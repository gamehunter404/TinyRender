#pragma once
#include"tgaImage.h"
#include"Maths.h"
#include<vector>

struct Mat4x4;
struct Vec3f;

//����������㷨
enum TriangleFillAlgorithm
{
	TRIFILL_SWEEPING = 0,
	TRIFILL_EDGEEQUATION,
};

//ͶӰ����
enum ProjectionType
{
	PROJECTION_PERSPECTIVE,//͸��ͶӰ
	PROJECTION_ORTHOGONAL,//����ͶӰ
};


struct Device
{
public:
	unsigned int** frameBuf;
	float** zBuf;
	int width;
	int height;

	Vec3f light_Dir =Vec3f(0,0,-1);//��ķ���

	float screenRatio;
	float nearPlane = 1.f;//��ƽ��
	float farPlane = 1000.;//Զƽ��
	float fov = 55;//��ֱ����
	float l, r, t, b;
	float light_intensity;

	ProjectionType projectionType = ProjectionType::PROJECTION_PERSPECTIVE;//ͶӰ����
	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;

};