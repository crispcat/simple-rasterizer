#ifndef SIMPLE_RASTERIZER_OBJMODEL_H
#define SIMPLE_RASTERIZER_OBJMODEL_H

#include <vector>
#include <string>
#include "geometry.h"

class ObjModel
{
public:
    std::vector<Vec3Float> vertices{};
    std::vector<Vec3Float> uvs{};
    std::vector<Vec3Float> normals{};
    std::vector<Vec3Int> faces{};
    explicit ObjModel(const std::string &path);
private:
    void parse_face_vertx(std::istringstream &s);
};

#endif //SIMPLE_RASTERIZER_OBJMODEL_H
