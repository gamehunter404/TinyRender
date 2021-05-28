#include<windows.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <tchar.h>
#include<iostream>
#include"Render.h"
#include"Model.h"
#include"Maths.h"
#include<vector>
#include"Device.h"
#include"TextureManager.h"
#include"tgaImage.h"
#include"Camera.h"

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif


int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP
unsigned char* screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

Color red = Color(255, 0, 0, 255);
Color green = Color(0, 255, 0, 255);
Color blue = Color(0, 0, 255, 255);
Color white = Color(255, 255, 255, 255);
Color black = Color(0,0,0,0);

const char* pf_TriangleObj = "Resources/triangle.obj";
const char* pf_cubeObj = "Resources/cube.obj";

const char* pf_AfricanHead = "Resources/african_head.obj";
const char* pf_AfricanHeadTexture = "Resources/african_head_diffuse.tga";
const char* pf_AfricanHeadNormalMap = "Resources/african_head_nm.tga";;
const char* pf_AfricanHeadSpecTexture = "Resources/african_head_spec.tga";
const char* pf_AfricanHeadTangentNormalTexture = "Resources/african_head_nm_tangent.tga";

const char* pf_DiabloObj = "Resources/diablo3_pose.obj";
const char* pf_DiablTexture = "Resources/diablo3_pose_diffuse.tga";
const char* pf_DiabloNormalTexture = "Resources/diablo3_pose_nm.tga";
const char* pf_DiabloSpecTexture = "Resources/diablo3_pose_spec.tga";
const char* pf_DiabloTangentNormalTexture = "Resources/diablo3_pose_nm_tangent.tga";

const char* pf_OutputTexture = "Resources/withShadow.tga";

int screen_init(int w, int h, const TCHAR* title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(void);							// 显示 FrameBuffer
void device_init(Device*device);
void device_destory(Device*device);

// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);


// 初始化窗口并设置标题
int screen_init(int w, int h, const wchar_t* title) {
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		w * h * 4, 0, 0, 0, 0 } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * 4);

	return 0;
}
int screen_close(void) {
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}
void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}
void screen_update(void) {
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}
void device_init(Device* device)
{
	if (device == nullptr) return;

	long long need = screen_w * screen_h * sizeof(float)*2 + screen_h * 3* sizeof(void*);
	unsigned char* ptr = new unsigned char[need];

	device->frameBuf = (unsigned int**)(ptr);
	device->zBuf = (float**)(ptr + screen_h * sizeof(void*));
	device->shadowBuf = (float**)(ptr + 2*screen_h*sizeof(void*));

	ptr += sizeof(void*) * screen_h * 3;
	unsigned char* shadowBufPtr = ptr + sizeof(float)*screen_h*screen_w;

	for (int i = 0; i < screen_h; i++)
	{
		device->frameBuf[i] = (unsigned int*)(screen_fb + 4 * i * screen_w);
		device->zBuf[i] = (float*)(ptr + sizeof(float) * screen_w * i);
		device->shadowBuf[i] = (float*)(shadowBufPtr+sizeof(float)*screen_w*i);
	}

	device->screen_width = screen_w;
	device->screen_height = screen_h;
	device->screenRatio = screen_w / (float)screen_h;

	if (device->projectionType == ProjectionType::PROJECTION_PERSPECTIVE) {
		device->t = std::tan(angleToRadians(device->fov * 0.5f)) * device->nearPlane;
		device->b = -device->t;
		device->r = device->screenRatio * device->t;
		device->l = -device->r;
	}
	else {
		device->t = 1.5f;
		device->b = -device->t;
		device->r = device->screenRatio*device->t;
		device->l = -device->r;
	}
	

}
void device_destory(Device* device)
{
	if (device == nullptr) return;

	if (device->frameBuf != nullptr) {

		delete[] device->frameBuf;
	}

	//zBuf 不需要delete ，否咋会造成重复delete

	device->zBuf = nullptr;
	device->frameBuf = nullptr;
	device->shadowBuf = nullptr;
}
void initApp()
{
	device_init(&gl_Device);

	TextureManager::initManager();
}
void destoryApp()
{
	device_destory(&gl_Device);

	TextureManager::destoryManager();
}
void clearBuffer(Device& device)
{
	unsigned int* fb = (unsigned int*)screen_fb;
	int nums = screen_w * screen_h;

	for (int i = 0; i < nums; i++)
		fb[i] = black.Data();

	float* zbuf = device.zBuf[0];
	float* shadowBuf = device.shadowBuf[0];
	nums = screen_w * screen_h;

	for (int i = 0; i < nums; i++)
	{
		zbuf[i] = FLT_MAX;
		shadowBuf[i] = FLT_MAX;
	}

}

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}


void renderToTgaFile(const char* filePath,Device&device)
{
	TGAImage image(device.screen_width, device.screen_height, 4);

	for (int x = 0; x < device.screen_width; x++)
	{
		for (int y = 0; y < device.screen_height; y++)
		{
			unsigned int color = device.frameBuf[y][x];//0xffffffff;
			unsigned char r, g, b, a;
			a = (0xff000000 & color)>>24;
			r = (0x00ff0000 & color)>>16;
			g = (0x0000ff00 & color)>>8;
			b = 0x000000ff & color;
			image.set(x, y, TGAColor(r, g, b, a));
		}
	}

	image.write_tga_file(filePath);

}
void renderDepthBuffer(Device&device)
{
	float* depthBuf = device.zBuf[0];
	unsigned int* frameBuf = device.frameBuf[0];
	int nums = device.screen_width * device.screen_height;

	Color color = black;

	for (int i = 0; i < nums; i++)
	{
		if (std::abs(depthBuf[i]) <1.0f)
		{

			Color color = Color(255, 255, 255, 255*std::abs(depthBuf[i]));
			frameBuf[i] = color.Data();
		}
	}

}
void setAfricanHeadModel(Model&model)
{
	model.setTexture(pf_AfricanHeadTexture);
	model.setNormalTexture(pf_AfricanHeadNormalMap);
	model.setSpecTexture(pf_AfricanHeadSpecTexture);
	model.setTangentTextureName(pf_AfricanHeadTangentNormalTexture);
	model.readObjFile(pf_AfricanHead);
}
void setCubeModel(Model& model)
{
	model.readObjFile(pf_cubeObj);
}

void setDiabloModel(Model& model)
{
	model.setTexture(pf_DiablTexture);
	model.setNormalTexture(pf_DiabloNormalTexture);
	model.setSpecTexture(pf_DiabloSpecTexture);
	model.setTangentTextureName(pf_DiabloTangentNormalTexture);
	model.readObjFile(pf_DiabloObj);
}

int main()
{
	const wchar_t* title = _T("TinyRender - ")
		_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title))
		return -1;

	initApp();
	Render render;
	FlatShader flatShader;
	TextureShader textureShader;
	NormalMapShader normalMapShader;
	LightIntensityShader lightIntensityShader;
	PhongShader phongShader;
	TangentNormalShader tangentShader;
	DepthShader depthShader;
	ShadowShader shadowShader;
	float angle = 4;
	std::vector<Model> models(1);
	Camera camera(Vec3f(1,1,4), Vec3f(0, 0, 0), Vec3f(0, 1, 0));


	TextureManager::LoadTexture(pf_AfricanHeadTexture);
	TextureManager::LoadTexture(pf_AfricanHeadNormalMap);
	TextureManager::LoadTexture(pf_AfricanHeadSpecTexture);
	TextureManager::LoadTexture(pf_DiabloNormalTexture);
	TextureManager::LoadTexture(pf_DiabloSpecTexture);
	TextureManager::LoadTexture(pf_DiablTexture);
	TextureManager::LoadTexture(pf_DiabloTangentNormalTexture);
	TextureManager::LoadTexture(pf_AfricanHeadTangentNormalTexture);
	
	Vec4f pos = vec4f_SetPoint(camera.getPos());
	gl_Device.light_Dir = { -1,-1,-1 };
	gl_Device.light_Dir.normalize();
	gl_Device.viewPortMat = getViewPortMat(gl_Device.screen_width, gl_Device.screen_height);//视口矩阵
	gl_Device.shader = &shadowShader;


	setAfricanHeadModel(models[0]);
	//setDiabloModel(models[0]);
	//setCubeModel(models[0]);
	

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {

		screen_dispatch();
		clearBuffer(gl_Device);
		
		render.renderWithShader(camera, models);
		//Model& model = models[0];
		//model.rotation(0,angle,0);
		//std::cout << angle << std::endl;
		//pos = mat4x4_Mul_Vec4f(getRotationYMat(angleToRadians(angle)),pos);
		//camera.setPos(pos);
		//gl_Device.light_Dir = pos;
		//Mat4x4 rotation = getRotationYMat(angleToRadians(angle));
		
		//angle += 2;


		screen_update();
		Sleep(1);
	}
	renderToTgaFile(pf_OutputTexture,gl_Device);

	destoryApp();

	return 0;

}