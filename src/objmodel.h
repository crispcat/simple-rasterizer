#ifndef SIMPLE_RASTERIZER_OBJMODEL_H
#define SIMPLE_RASTERIZER_OBJMODEL_H

#include <vector>
#include <string>
#include "geometry.h"

typedef Vector3<Vec3Int> Face;

class ObjModel {
public:
        ObjModel(const std::string &path);
private:
        std::vector<Vec3Float> m_vertices {};
        std::vector<Vec3Float> m_uvs {};
        std::vector<Vec3Float> m_normals {};
        std::vector<Face> m_faces {};
};

#endif //SIMPLE_RASTERIZER_OBJMODEL_H
