#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "ObjModel.h"

ObjModel::ObjModel()
{
}

ObjModel::ObjModel(const char* filename) : _verts(), _faces() {
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
            float x, y, z;
            iss >> x >> y >> z;

            _verts.push_back({x,y,z});
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
        else if (!line.compare(0, 3, "vn "))
        {
            Vec3f vn;
            iss >> trash >> trash;
            iss >> vn.x >> vn.y >> vn.z;
            _vns.push_back(vn);
        }
    }
    std::cerr << "# v# " << _verts.size() << " f# " << _faces.size() << std::endl;
}

ObjModel::~ObjModel() {
}

int ObjModel::nverts() {
    return (int)_verts.size();
}

int ObjModel::nfaces() {
    return (int)_faces.size();
}

int ObjModel::nuvs()
{
    return _uvs.size();
}

int ObjModel::nvns()
{
    return _vns.size();
}

std::vector<Vertex> ObjModel::face(int idx) {
    return _faces[idx];
}

void ObjModel::translate(float x, float y, float z)
{
    _translate.x = x;
    _translate.y = y;
    _translate.z = z;
}

void ObjModel::scale(float x, float y, float z)
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;
}

void ObjModel::rotation(float x, float y, float z)
{
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;
}

Vec3f ObjModel::getTranslate() const
{
    return _translate;
}

Vec3f ObjModel::getScale() const
{
    return _scale;
}

Vec3f ObjModel::getRotation() const
{
    return _rotation;
}

std::string ObjModel::getTextureName() const
{
    return _texName;
}

std::string ObjModel::getNormalTextureName() const
{
    return _normalTexame;
}

std::string ObjModel::getSpecTextureName() const
{
    return _specTexName;
}

std::string ObjModel::getTangentTextureName() const
{
    return _tangentTexName;
}

void ObjModel::readObjFile(const char* filename)
{
    _verts.clear();
    _faces.clear();

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
            int itrash, idx, vti, vni;
            iss >> trash;
            while (iss >> idx >> trash >> vti >> trash >> vni) {
                idx--; // in wavefront obj all indices start at 1, not zero
                vti--;
                vni--;
                f.push_back({ idx,vti,vni });
            }
            _faces.push_back(f);
        }
        else if (!line.compare(0, 3, "vt "))
        {
            Vec3f vt;
            iss >> trash >> trash;
            iss >> vt.x >> vt.y >> vt.z;
            _uvs.push_back(vt);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            Vec3f vn;
            iss >> trash >> trash;
            iss >> vn.x >> vn.y >> vn.z;
            _vns.push_back(vn);
        }
    }
    std::cerr << "# v# " << _verts.size() << " f# " << _faces.size() << std::endl;
}


void ObjModel::setTexture(const std::string& texname)
{
    _texName = texname;
}

void ObjModel::setNormalTexture(const std::string& name)
{
    _normalTexame = name;
}

void ObjModel::setTangentTextureName(const std::string& name)
{
    _tangentTexName = name;
}

void ObjModel::setSpecTexture(const std::string& name)
{
    _specTexName = name;
}

Vec3f ObjModel::vert(int i) {
    return _verts[i];
}

Vec3f ObjModel::vert(int iface, int nthVert)
{
    auto& face = _faces[iface];
    return _verts[face[nthVert].verIndex];
}

Vec3f ObjModel::uvs(int i)
{
    return _uvs[i];
}

Vec3f ObjModel::uvs(int iface, int nth)
{
    Vertex& indices = _faces[iface].at(nth);
    return _uvs[indices.vtIndex];
}

Vec3f ObjModel::vns(int i)
{
    return _vns[i];
}

Vec3f ObjModel::vns(int iface, int nth)
{
    Vertex& indices = _faces[iface].at(nth);
    return _vns[indices.vnIndex];
}
