#ifndef SIMPLE_RASTERIZER_CONTEXT_H
#define SIMPLE_RASTERIZER_CONTEXT_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

typedef uint32_t Color;
const Color COLOR_WHITE = 0xfafafa;
const Color COLOR_RED   = 0xfa0000;
const Color COLOR_GREEN = 0x00fa00;
const Color COLOR_BLUE  = 0x0000fa;
const Color COLOR_BLACK = 0x000000;

typedef Vector2<uint16_t> ScreenPoint;

struct ScreenLine
{
    bool flipped;
    uint16_t x;
    uint16_t y;
    uint16_t x1;
    int dev;
    int ddev;
    int _2dy;
    int diry;
    int xstep;
    ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool move();
};

/*
 * GENERAL ABSTRACT CONTEXT
 * */
class RenderContext
{
public:
    std::vector<Vec3Float> vertices {};
    std::vector<Vec3Float> uvs {};
    std::vector<Vec3Float> normals {};
    std::vector<Vec3Int> faces {};
    uint32_t color { COLOR_BLACK };
    explicit RenderContext(Vec3Float scale_vector) : scale_vector(scale_vector) {};
    virtual ~RenderContext() = default;
public:
    void points();
    void lines();
    void triangles();
    void triangles_wired();
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
protected:
    Vec3Float scale_vector;
    virtual void pixel(uint16_t x, uint16_t y, bool flipped) = 0;
    ScreenPoint vertex_transform2screen(Vec3Float v);
};

/*
 * REALTIME CONTEXT
 * */
class RtContext : public RenderContext
{
public:
    uint32_t *f_buff;
    explicit RtContext(uint32_t *f_buff, uint16_t w, uint16_t h);
protected:
    void pixel(uint16_t x, uint16_t y, bool flipped) override;
private:
    uint16_t w;
    uint16_t h;
};

/*
 * TGA IMAGE CONTEXT
 * */
class TgaImageContext : public RenderContext
{
public:
    TgaImage &image;
    explicit TgaImageContext(TgaImage &image);
protected:
    void pixel(uint16_t x, uint16_t y, bool flipped) override;
private:
    uint16_t h;
};

#endif //SIMPLE_RASTERIZER_CONTEXT_H
