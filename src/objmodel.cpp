#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "objmodel.h"

ObjModel::ObjModel(const std::string &path)
{
    std::cout << "Start parsing model file " << path << '\n';
    std::ifstream in;
    in.open(path, std::ios::in);
    if (in.fail())
    {
        std::cerr << "Unable to open filestream " << path << '\n';
        return;
    }

    std::string line;
    std::string token;
    int lineCount = 0;
    Vec3 vec3f;
    Vec2 vec2f;
    while (!in.eof())
    {
        lineCount++;
        std::getline(in, line);
        std::istringstream s(line);
        if (s >> token)
        {
            if (token == "v")
            {
                s >> vec3f;
                vertices.push_back(vec3f);
            }
            else if (token == "vt")
            {
                s >> vec2f;
                uvs.push_back(vec2f);
            }
            else if (token == "vn")
            {
                s >> vec3f;
                normals.push_back(-vec3f);
            }
            else if (token == "f")
            {
                parse_face_vertx(s);
            }
            else if (token == "#diffuse")
            {
                s >> token;
                load_texture(diffuse, path, token);
            }
            else
            {
                std::cout << "\tNot defined token " << '"' << token << '"' << " at line " << lineCount << " skipping "
                          << '"' << line << '"' << '\n';
            }
        }
    }

    std::cout << "Model file " << path << " parsed!" << '\n';
    std::cout << "\tVertices parsed: " << vertices.size() << '\n';
    std::cout << "\tUVs parsed: " << uvs.size() << '\n';
    std::cout << "\tNormals parsed: " << normals.size() << '\n';
    std::cout << "\tFaces parsed: " << faces.size() << '\n';
}

void ObjModel::load_texture(TgaImage &tex, const std::string &path, const std::string &name)
{
    auto p = std::filesystem::path(path).parent_path().append(name).string();
    tex.read_tga_file(p.c_str());
    tex.flip_vertically();
    std::cout << "\tTexture " << p << " loaded. Size " << tex.get_width() << 'x' << tex.get_height() << ".\n";
}

void ObjModel::parse_face_vertx(std::istringstream &s)
{
    char ch;
    Vec3Int ivec = Vec3Int(-1, -1, -1);
    while (s >> ivec.iv)
    {
        if (s.peek() == '/')
        {
            s >> ch;
            if (s.peek() == '/')
                s >> ch >> ivec.in;
            else
                s >> ivec.iuv >> ch >> ivec.in;
        }
        faces.push_back(ivec - Vec3::one()); // sub to make indexes start from 0
    }
}
