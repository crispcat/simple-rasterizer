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
    void triangle_bound(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    virtual void pixel(uint16_t x, uint16_t y) = 0;

protected:
    typedef Vector2<uint16_t> ScreenPoint;
    struct ScreenLine
    {
        uint16_t *x;
        uint16_t *y;
        bool flipped;
        uint16_t _x;
        uint16_t _y;
        uint16_t _x1;
        int dev;
        int ddev;
        int _2dy;
        int diry;
        int dirx;
        ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
        bool move();
    };

protected:
    Vec3Float scale_vector;
    ScreenPoint transform2screen(Vec3Float v);
};

/*
 * REALTIME CONTEXT
 * */
class RtContext : public RenderContext
{
public:
    explicit RtContext(uint32_t *f_buff, uint16_t w, uint16_t h) :
            RenderContext({(float)w / 2, (float)h / 2, 1.f }),
            f_buff(f_buff),
            w(w),
            h(h)
            { }
    void pixel(uint16_t x, uint16_t y) override { f_buff[(h - y) * w + x] = color; }

private:
    uint32_t *f_buff;
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
    explicit TgaImageContext(TgaImage &image) :
            RenderContext({(float)image.get_width() / 2, (float)image.get_height() / 2, 1.f}),
            image(image),
            h(image.get_height())
            { }
    void pixel(uint16_t x, uint16_t y) override { image.set(x, h - y, color); }

private:
    uint16_t h;
};

#endif //SIMPLE_RASTERIZER_CONTEXT_H