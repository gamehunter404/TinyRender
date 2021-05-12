#pragma once
#include"tgaImage.h"
#include<vector>

//һЩ��Ⱦ����
enum TriangleFillAlgorithm
{
	TRIFILL_SWEEPING = 0,
	TRIFILL_EDGEEQUATION,
};

struct Device
{
public:
	unsigned int** frameBuf;
	float** zBuf;
	TGAImage* texture;
	int width;
	int height;
	float light_intensity;


	TriangleFillAlgorithm triangleFillSetting = TriangleFillAlgorithm::TRIFILL_EDGEEQUATION;
};