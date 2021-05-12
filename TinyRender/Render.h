#pragma once
#include"Maths.h"

struct Device;

struct Color
{

public:
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_color = (a << 24) | (r<<16)|(g<<8)|(b);

		float div = (float)a / 255;
		r *= div;
		g *= div;
		b *= div;

		_acolor = (0xff000000) | (r << 16) | (g << 8) | (b);
	}

	Color(unsigned char r_, unsigned char g_, unsigned char b_):r(r),g(g_),b(b_),a(255)
	{

	}

	unsigned int RawData() const
	{
		return _color;
	}

	unsigned int Data() const
	{
		return _acolor;
	}


public:
	union {
		unsigned int _color;
		unsigned char a, r, g, b;
	};

	unsigned int _acolor;
};

class Render
{
public:
	
	void WireframeRender(const Device&device);
	void SetPixel(int x,int y,const Color&color,Device&device);
	void DrawLine(int x0,int y0,int x1,int y1, const Color& color,Device&device);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Color&color,Device&device);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Vec3f* uvs, Device& device);

private:

	/// <summary>
	/// 使用内外测试来进行三角形填充
	/// </summary>
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,const Color&color,Device&device);
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,Vec3f*uvs, Device& device);
	/// <summary>
	/// 使用扫描线算法来进行三角形的绘制
	/// </summary>
	void drawTriBySweeping(Vec3f* w, Vec2Int* v,const Color&color,Device&device);

	Vec3f interpolateUVCoord(Vec3f*uv,const Vec3f& vuw);

private:

	/// <summary>
	/// 这种在叉乘的计算方式只适用于二维的三角形
	/// </summary>
	Vec3f getBarycentric2D(const Vec2Int&v0,const Vec2Int&v1,const Vec2Int& v2 ,const Vec2Int& p);
};