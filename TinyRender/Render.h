#pragma once
#include"Maths.h"

struct Device;

struct Color
{

public:
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		//Ð¡¶Ë×Ö½ÚÐò
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
};