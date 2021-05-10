#pragma once

#include<vector>

//“ª–©‰÷»æ…Ë÷√
enum TriangleFillSetting
{
	TRIFILL_SWEEPING = 0,
	TRIFILL_EDGEEQUATION,
};


struct Device
{
public:
	unsigned int** frameBuf;
	float** zBuf;
	int width;
	int height;

	TriangleFillSetting triangleFillSetting = TriangleFillSetting::TRIFILL_EDGEEQUATION;
};