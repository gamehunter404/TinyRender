#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model()
{
}

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

int Model::nvns()
{
    return _vns.size();
}

std::vector<Vertex> Model::face(int idx) {
    return _faces[idx];
}

void Model::translate(float x, float y, float z)
{
    _translate.x = x;
    _translate.y = y;
    _translate.z = z;
}

void Model::scale(float x, float y, float z)
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;
}

void Model::rotation(float x, float y, float z)
{
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;
}

Vec3f Model::getTranslate() const
{
    return _translate;
}

Vec3f Model::getScale() const
{
    return _scale;
}

Vec3f Model::getRotation() const
{
    return _rotation;
}

std::string Model::getTextureName() const
{
    return _texName;
}

std::string Model::getNormalTextureName() const
{
    return _normalTexame;
}

std::string Model::getSpecTextureName() const
{
    return _specTexName;
}

std::string Model::getTangentTextureName() const
{
    return _tangentTexName;
}

void Model::readObjFile(const char* filename)
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


void Model::setTexture(const std::string& texname)
{
    _texName = texname;
}

void Model::setNormalTexture(const std::string& name)
{
    _normalTexame = name;
}

void Model::setTangentTextureName(const std::string& name)
{
    _tangentTexName = name;
}

void Model::setSpecTexture(const std::string& name)
{
    _specTexName = name;
}

Vec3f Model::vert(int i) {
    return _verts[i];
}

Vec3f Model::vert(int iface, int nthVert)
{
    auto& face = _faces[iface];
    return _verts[face[nthVert].verIndex];
}

Vec3f Model::uvs(int i)
{
    return _uvs[i];
}

Vec3f Model::uvs(int iface, int nth)
{
    Vertex& indices = _faces[iface].at(nth);
    return _uvs[indices.vtIndex];
}

Vec3f Model::vns(int i)
{
    return _vns[i];
}

Vec3f Model::vns(int iface, int nth)
{
    Vertex& indices = _faces[iface].at(nth);
    return _vns[indices.vnIndex];
}
