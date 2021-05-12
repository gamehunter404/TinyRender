#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include"Maths.h"

struct Vertex
{
	int verIndex;
	int vtIndex;
	int vnIndex;
};

class Model {
private:
	std::vector<Vec3f> _verts;
	std::vector<Vec3f> _uvs;
	std::vector<std::vector<Vertex> > _faces;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int nuvs();
	Vec3f vert(int i);
	Vec3f uvs(int i);
	std::vector<Vertex> face(int idx);
	
};

#endif //__MODEL_H__