#ifndef SIMPLE_RASTERIZER_CONTEXT_H
#define SIMPLE_RASTERIZER_CONTEXT_H

#include <vector>
#include "tgaimage.h"
#include "geometry.h"

typedef Vector2<uint16_t> ScreenPoint;

class TgaImageContext
{
public:
    TGAImage &tgaImage;
    TGAColor tgaColor;
    std::vector<Vec3Float> vertices {};
    std::vector<Vec3Float> uvs {};
    std::vector<Vec3Float> normals {};
    std::vector<Vec3Int> faces {};
public:
    explicit TgaImageContext(TGAImage &i);
    void SetImage(TGAImage &i);
    void points();
    void lines();
    void triangles();
    void triangles_w();
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void triangle_old(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void w_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
private:
    Vec3Float scaleVector;
    void pixel(uint16_t x, uint16_t y, bool flipped);
    ScreenPoint vertex_transform2screen(Vec3Float v);
};

struct ScreenLine
{
    bool flipped;
    uint16_t x;
    uint16_t y;
    uint16_t x1;
    int diry;
    int dev;
    int ddev;
    int _2dy;

    ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool move();
};

#endif //SIMPLE_RASTERIZER_CONTEXT_H
