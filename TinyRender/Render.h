#pragma once
#include"Maths.h"

struct Device;

struct Color
{

public:
	Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		//小端字节序
		_color = (a << 24) | (r<<16)|(g<<8)|(b);
	}
	

	operator unsigned int() const
	{
		return _color;
	}

private:
	unsigned int _color;
};

class Render
{
public:
	
	void WireframeRender(const Device&device);
	void SetPixel(int x,int y,const Color&color,Device&device);
	void DrawLine(int x0,int y0,int x1,int y1, const Color& color,Device&device);
	void DrawTriangle(Vec2Int v0, Vec2Int v1, Vec2Int v2,const Color&color,Device&device);

private:

	/// <summary>
	/// 使用内外测试来进行三角形填充
	/// </summary>
	void drawTriByEdgeEquation(Vec2Int v0,Vec2Int v1,Vec2Int v2,const Color&color,Device&device);
	/// <summary>
	/// 使用扫描线算法来进行三角形的绘制
	/// </summary>
	void drawTriBySweeping(Vec2Int v0,Vec2Int v1,Vec2Int v2,const Color&color,Device&device);

private:

	/// <summary>
	/// 这种在叉乘的计算方式只适用于二维的三角形
	/// </summary>
	Vec3f getBarycentric2D(const Vec2Int&v0,const Vec2Int&v1,const Vec2Int& v2 ,const Vec2Int& p);
};