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

Color Model::getColorFromTexture(float u, float v) const
{

    int x = texture.get_width()*u;
    int y = texture.get_height()*v;
    TGAColor tc = texture.get(x,y);
    return Color(tc.bgra[2],tc.bgra[1],tc.bgra[0],255);
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
    }
    std::cerr << "# v# " << _verts.size() << " f# " << _faces.size() << std::endl;
}

bool Model::loadTextureFromTGA(const char* filename)
{
    if (filename == nullptr) {
        return false;
    }

    bool res = true;

    std::cerr << std::endl<<"-------------------------------"<<std::endl;

    if (texture.read_tga_file(filename)) {

        std::cerr << "successed to load texture "<<"' "<< filename <<"'"<<std::endl;
    }
    else {
        std::cerr << "failed to load texture " << "' " << filename << "'" << std::endl;
        res = false;
    }

    std::cerr << std::endl << "-------------------------------" << std::endl;


    return res;
}

void Model::setTexture(const TGAImage& texture_)
{
    texture = texture_;
}



Vec3f Model::vert(int i) {
    return _verts[i];
}

Vec3f Model::uvs(int i)
{
    return _uvs[i];
}

Vec3f Model::vns(int i)
{

    return _vns[i];
}
