#pragma once
#include<vector>

struct Color;
struct Vec3f;
struct Vec4f;
struct ObjModel;
struct Device;
class Camera;
struct Device;
struct Vec2Int;
class IShader;

class Render
{
public:
	void SetPixel(int x,int y,const Color&color);
	void DrawLine(int x0,int y0,int x1,int y1, const Color& color);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Color&color);
	void DrawTriangle(Vec3f* w, Vec2Int* v, Vec3f* uvs);
	void renderWithShader(const Camera& camera, std::vector<ObjModel>& models);
	void renderModel(const Camera& camera,std::vector<ObjModel>&models);

private:
	void renderShadow(ObjModel& model,Device&device);
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,const Color&color);
	void drawTriByEdgeEquation(Vec3f* w, Vec2Int* v,Vec3f*uvs);
	void drawTriByEdgeEquation(Vec4f* w, Vec4f* v, Vec3f* uvs,Vec3f*vns,const ObjModel&model);
	void rasterizedTriangle(Vec4f*w,IShader&shader);
	void drawTriBySweeping(Vec3f* w, Vec2Int* v,const Color&color);
	Vec3f interpolateUVCoord(Vec3f*uv,const Vec3f& vuw);
	Vec3f getBarycentric2D(const Vec2Int& v0, const Vec2Int& v1, const Vec2Int& v2, const Vec2Int& p);
	bool isBackFace(const Vec4f& v0,const Vec4f&v1,const Vec4f&v2);
};