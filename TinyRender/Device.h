#ifndef __DEVICE_H__
#define __DEVICE_H__

#include"tgaImage.h"
#include"Maths.h"
#include"Shader.h"
#include"Model.h"
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


enum FaceCulling
{
	FACECULLING_NONE = 0,
	FACECULLING_BACK,
	FACECULLING_FRONT
};

struct Device
{
public:
	unsigned int** frameBuf = nullptr;
	float** zBuf = nullptr;
	int screen_width;
	int screen_height;
	float screenRatio;
	float nearPlane = 0.1f;
	float farPlane = 10000.f;
	float fov = 35;//vertical filed  of view
	float l, r, t, b;

	Vec4f varying_w[3];//world space
	Vec4f varying_V[3];// window space
	Vec3f varying_uvs[3];//uv coordinate
	Vec3f varying_vns[3];//normals , the normal may be transform
	Vec3f varying_eyePos;

	Mat4x4 uniform_vpMat;
	Mat4x4 viewPortMat;
	Mat4x4 viewMat;
	Mat4x4 modelMat;
	Mat4x4 inverseModelMat;
	

	Model* model = nullptr;
	IShader* shader = nullptr;
	TGAImage* texture = nullptr;
	TGAImage* normalTexture = nullptr;// normal Map
	TGAImage* specularTexture = nullptr;

	Vec3f light_Dir = Vec3f(0,0,-1);//光的方向
	float varying_PixelDepth;

	bool depthTest = true;
	FaceCulling faceCulling = FaceCulling::FACECULLING_BACK;
	ProjectionType projectionType = ProjectionType::PROJECTION_PERSPECTIVE;//投影类型
	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;
};

extern Device gl_Device;

#endif // !__DEVICE_H__
