#include"Render.h"
#include"Device.h"



void Render::WireframeRender(const Device& device)
{



}

void Render::SetPixel(int x, int y, const Color& color, Device& device)
{
	if (x < 0 || x >= device.width) return;
	if (y<0 || y>=device.height) return;

	// i = y,j = x
	device.frameBuf[y][x] = color;
}



void Render::DrawLine(int x0, int y0, int x1, int y1, Color& color,Device&device)
{
	bool isStep = false;

	if (std::abs(x0 - x1) < std::abs(y1 - y0))
	{
		isStep = true;
		std::swap(x0,y0);
		std::swap(x1,y1);
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

	for (int x = x0,y = y0; x <= x1; x++)
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

