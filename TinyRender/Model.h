#ifndef __MODEL_H__
#define __MODEL_H__

#include"Maths.h"
#include"tgaImage.h"
#include <vector>


struct Vertex
{
	int verIndex;
	int vtIndex;
	int vnIndex;
};

class Model {
private:
	std::vector<Vec3f> _verts;//顶点坐标
	std::vector<Vec3f> _uvs;//顶点uv
	std::vector<std::vector<Vertex>> _faces;//一个三角面
	std::vector<Vec3f> _vns;//顶点法线
	TGAImage texture;

	Vec3f _scale = Vec3f(1,1,1);
	Vec3f _rotation;
	Vec3f _translate;




public:
	Model();
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int nuvs();
	int nvns();
	Vec3f vert(int i);
	Vec3f uvs(int i);
	Vec3f vns(int i);
	std::vector<Vertex> face(int idx);
	
	//旋转，缩放，平移
	void translate(float x,float y,float z);
	void scale(float x,float y,float z);
	void rotation(float x, float y, float z);

	Vec3f getTranslate() const;
	Vec3f getScale() const;
	Vec3f getRotation() const;
	Color getColorFromTexture(float u,float v) const;

	void readObjFile(const char* filename);
	bool loadTextureFromTGA(const char*filename);
	void setTexture(const TGAImage&texture_);
};

#endif //__MODEL_H__