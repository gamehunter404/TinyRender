#pragma once
#include"tgaImage.h"
#include"Maths.h"

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
	float screenRatio;
	float nearPlane = 0.1f;//��ƽ��
	float farPlane = 10000.f;//Զƽ��
	float fov = 60;//��ֱ����
	float l, r, t, b;

	Mat4x4 vpMat;
	Mat4x4 viewPortMat;
	Mat4x4 viewMat;
	Mat4x4 modelMat;
	Mat4x4 inverseModelMat;

	TGAImage* texture;
	int texWidth;
	int texHeight;

	Vec3f light_Dir = Vec3f(0, 0, -1);//��ķ���

	ProjectionType projectionType = ProjectionType::PROJECTION_PERSPECTIVE;//ͶӰ����
	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;

};