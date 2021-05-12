#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : _verts(), _faces() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            iss >> v.x >> v.y >> v.z;
            _verts.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<Vertex> f;
            int itrash, idx,vti,vni;
            iss >> trash;
            while (iss >> idx >> trash >> vti >> trash >> vni) {
                idx--; // in wavefront obj all indices start at 1, not zero
                vti--;
                vni--;
                f.push_back({idx,vti,vni});
            }
            _faces.push_back(f);
        }
        else if (!line.compare(0, 3, "vt "))
        {
            Vec3f vt;
            iss >> trash>>trash;
            iss >> vt.x >> vt.y >> vt.z;
            _uvs.push_back(vt);
        }
    }
    std::cerr << "# v# " << _verts.size() << " f# " << _faces.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)_verts.size();
}

int Model::nfaces() {
    return (int)_faces.size();
}

int Model::nuvs()
{
    return _uvs.size();
}

std::vector<Vertex> Model::face(int idx) {
    return _faces[idx];
}

Vec3f Model::vert(int i) {
    return _verts[i];
}

Vec3f Model::uvs(int i)
{
    return _uvs[i];
}
