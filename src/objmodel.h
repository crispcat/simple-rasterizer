#ifndef SIMPLE_RASTERIZER_OBJMODEL_H
#define SIMPLE_RASTERIZER_OBJMODEL_H

#include <vector>
#include <string>
#include "tgaimage.h"
#include "math/vector.h"

class ObjModel
{
public:
    TgaImage diffuse {};
    std::vector<Vec3> vertices {};
    std::vector<Vec2> uvs {};
    std::vector<Vec3> normals {};
    std::vector<Vec3Int> faces {};
    explicit ObjModel(const std::string &path);
private:
    void parse_face_vertx(std::istringstream &s);
    static void load_texture(TgaImage &tex, const std::string &path, const std::string &name);
};

#endif //SIMPLE_RASTERIZER_OBJMODEL_H