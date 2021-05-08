#pragma once

struct Device;

struct Vector2
{
	int x, y;
};

struct Color
{

public:
	Color(char r, char g, char b, char a)
	{
		_color = (r << 24) | (g<<16)|(b<<8)|(a);
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
	void DrawLine(int x0,int y0,int x1,int y1, Color& color,Device&device);

};