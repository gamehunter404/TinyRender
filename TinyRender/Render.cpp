#include"Render.h"
#include"Device.h"
#include<iostream>


void Render::WireframeRender(const Device& device)
{



}

void Render::SetPixel(int x, int y, const Color& color, Device& device)
{
	if (x < 0 || x >= device.width) return;
	if (y<0 || y>=device.height) return;

	// i = y,j = x
	device.frameBuf[y][x] = color.Data();
}

void Render::DrawLine(int x0, int y0, int x1, int y1,const Color& color,Device&device)
{
	bool isStep = false;

	if (std::abs(x0 - x1) < std::abs(y1 - y0))
	{
		isStep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}


	if (x0 > x1)
	{
		std::swap(x0,x1);
		std::swap(y1,y0);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = 2*std::abs(dy);
	int error2 = 0;

	

	for (int x = x0,y = y0; x <=x1; x++)
	{
		if (isStep) {
			SetPixel(y,x,color,device);
		}
		else {
			SetPixel(x,y,color,device);
		}

		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Render::DrawTriangle(Vec3f*w, Vec2Int*v, Color& color, Device& device)
{

	switch (device.triangleFillSetting)
	{
	case TriangleFillAlgorithm::TRIFILL_SWEEPING:
		drawTriBySweeping(w,v,color,device);
		break;

	case TriangleFillAlgorithm::TRIFILL_EDGEEQUATION:
		drawTriByEdgeEquation(w,v,color,device);
		break;
	}

}



void Render::drawTriByEdgeEquation(Vec3f* w, Vec2Int* v, const Color& color, Device& device)
{

	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];

	int minX = std::min(std::min(v0.x,v1.x),v2.x), maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y), maxY = std::max(std::max(v0.y,v1.y),v2.y);
	
	minX = std::max(0,minX);
	maxX = std::min(device.width-1,maxX);
	minY = std::max(0,minY);
	maxY = std::min(device.height-1,maxY);

	if (v0.y == v1.y && v2.y == v1.y) {

		DrawLine(minX,v0.y,maxX,v0.y,color,device);
		return;
	}
	else if (v0.x == v1.x && v1.x == v2.x)
	{
		DrawLine(v0.x,minY,v0.x,maxY,color,device);
		return;
	}

	float z = 0;

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			Vec3f u = getBarycentric2D(v0, v1, v2, {x,y});


			if (u.x < 0 || u.y < 0 || u.z < 0) 
				continue;

			z = w[0].z * u.x+w[1].z*u.y+w[2].z*u.z;

			if (z > device.zBuf[y][x])
			{
				device.zBuf[y][x] = z;
				SetPixel(x, y, color, device);
			}

		}
	}
}

void Render::drawTriBySweeping(Vec3f* w, Vec2Int* v, const Color& color, Device& device)
{
	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];

	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);

	int x0 = v0.x, x1 = v0.x;

	if (v0.y == v1.y && v1.y == v2.y) {

		//三角形退化为一条水平直线
		x0 = std::min(std::min(v0.x, v1.x), v2.x);
		x1 = std::max(std::max(v0.x, v1.x), v2.x);
		DrawLine(x0, v0.y, x1, v0.y, color, device);
		return;
	}

	for (int y = v0.y; y <= v2.y; y++)
	{

		float t1 = (float)(y - v0.y) / (v2.y - v0.y);
		float t2 = 0;
		x0 = v0.x + t1 * (v2.x - v0.x);

		if (y >= v1.y)
		{
			t2 = (v2.y - v1.y) != 0 ? (float)(y - v1.y) / (v2.y - v1.y) : 0;
			x1 = v1.x + t2 * (v2.x - v1.x);

		}
		else {
			t2 = (float)(y - v0.y) / (v1.y - v0.y);
			x1 = v0.x + t2 * (v1.x - v0.x);
		}

		if (x0 > x1) std::swap(x0, x1);

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(x, y, color, device);
		}

	}
}

Vec3f Render::getBarycentric2D(const Vec2Int& v0, const Vec2Int& v1, const Vec2Int& v2, const Vec2Int& p)
{

	Vec3f u = Vec3f_Cross(Vec3f((v1.x - v0.x),(v2.x - v0.x),(v0.x - p.x)),
		Vec3f( (v1.y - v0.y),(v2.y - v0.y),(v0.y - p.y)));

	if (std::abs(u.z) < 1.f)
	{
		//u.z <1 表示u.z的值为0。此时表示三角形已经退化
		return {-1,1,1};
	}


	return { 1.f - (u.x + u.y) / (u.z) ,u.x/u.z,u.y/u.z};
}

