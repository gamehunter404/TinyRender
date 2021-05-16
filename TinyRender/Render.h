#pragma once
#include<vector>

struct Color;
struct Vec3f;
struct Vec4f;
struct Model;
struct Device;
class Camera;
struct Device;
struct Vec2Int;


class Render
{
public:
	
	void WireframeRender(const Device&device);
	void SetPixel(int x,int y,const Color&color,Device&device);
	void DrawLine(int x0,int y0,int x1,int y1, const Color& color,Device&device);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Color&color,Device&device);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Vec3f* uvs, Device& device);

	void renderModel(const Camera& camera,std::vector<Model>&models,Device&device);

private:

	/// <summary>
	/// ʹ������������������������
	/// </summary>
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,const Color&color,Device&device);
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,Vec3f*uvs, Device& device);
	void drawTriByEdgeEquation(Vec4f* w, Vec4f* v, Vec3f* uvs,Vec3f*vns,const Model&model ,Device& device);
	/// <summary>
	/// ʹ��ɨ�����㷨�����������εĻ���
	/// </summary>
	void drawTriBySweeping(Vec3f* w, Vec2Int* v,const Color&color,Device&device);

	Vec3f interpolateUVCoord(Vec3f*uv,const Vec3f& vuw);

private:

	/// <summary>
	/// �����ڲ�˵ļ��㷽ʽֻ�����ڶ�ά��������
	/// </summary>
	Vec3f getBarycentric2D(const Vec2Int&v0,const Vec2Int&v1,const Vec2Int& v2 ,const Vec2Int& p);
};