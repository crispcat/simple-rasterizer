#include <iostream>
#include <fstream>
#include <sstream>
#include "objmodel.h"

ObjModel::ObjModel(const std::string &path) {

        std::cout << "Start parsing model file " << path << '\n';

        std::ifstream in;
        in.open(path, std::ios::in);
        if (in.fail()) {
                std::cerr << "Unable to open filestream " << path << '\n';
                return;
        }

        std::string line;
        uint32_t nline = 0;
        std::string token;
        Vec3Float fvec;
        char32_t null;
        Face face;
        while (!in.eof()) {
                nline++;
                std::getline(in, line);
                std::istringstream s(line);
                if (s >> token) {
                        if (token == "v") {
                                s >> fvec;
                                m_vertices.push_back(fvec);
                        }
                        else if (token == "vt") {
                                s >> fvec;
                                m_uvs.push_back(fvec);
                        }
                        else if (token == "vn") {
                                s >> fvec;
                                m_normals.push_back(fvec);
                        }
                        else if (token == "f") {
                                for (Vec3Int &ivec : face.raw) {
                                        s >> ivec.ivert >> null >> ivec.iuv >> null >> ivec.inorm;
                                        ivec = ivec - Vec3IntOne;
                                }
                        }
                        else {
                                std::cout << "\tNot defined token " << '"' << token << '"' << " at line " << nline
                                          << " skipping " << '"' << line << '"' << '\n';
                        }
                }
        }

        std::cout << "Model file " << path << " parsed!" << '\n';
        std::cout << "\tVertices parsed: " << m_vertices.size() << '\n';
        std::cout << "\tUVs parsed: " << m_uvs.size() << '\n';
        std::cout << "\tNormals parsed: " << m_normals.size() << '\n';
}
