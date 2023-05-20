#ifndef SIMPLE_RASTERIZER_RENDERER_H
#define SIMPLE_RASTERIZER_RENDERER_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"
#include "renderer_structs.h"

/*
 * GENERAL ABSTRACT CONTEXT
 * */
class RenderContext
{
/*
 * STATE
 * */
public:
    std::vector<Vec3Float> vertices {};
    std::vector<Vec3Float> uvs {};
    std::vector<Vec3Float> normals {};
    std::vector<Vec3Int> faces {};
    TgaImage texture;

protected:
    Vec3Float pos(Vertex v) const { return vertices[v.iv]; }
    Vec3Float normal(Vertex v) const { return normals[v.in]; }
    Vec3Float uv(Vertex v) const { return uvs[v.iuv]; }
    Color32 texture_pixel(Vec3Float uv);

public:
    Color32 color;
    Vec3Float light_dir { 0.f, 0.f, -1.f };

public:
    explicit RenderContext(uint16_t w, uint16_t h, float distance);
    virtual ~RenderContext();
/*
 * 2D
 * */
public:
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/*
 * 3D
 * */
public:
    void frame();
    void points();
    void triangles();
    void triangles_wired();
protected:
    void triangle(Vertex face[3]);
/*
 * LIGHT
 * */
protected:
    void apply_texture(Frag &f) const;
    void flat_light(Frag &f) const ;
    void gouroud_light(Frag &f) const ;
    void fong_light(Frag &f) const;
/*
 * MISC
 * */
protected:
    uint16_t w;
    uint16_t h;
    float z_max;
    float *z_buff;
    bool have_ns;
    bool have_uvs;
    void frag(Frag &f);
    virtual void pixel(uint16_t x, uint16_t y, Color32 c) = 0;
    Vec3Float viewport_scale_vector;
    ScreenPoint transform2screen(Vec3Float v) const;
    Vec3Float texture_scale_vector;
};

/*
 * REALTIME CONTEXT
 * */
class RtContext : public RenderContext
{
public:
    explicit RtContext(uint32_t *f_buff, uint16_t w, uint16_t h, float dist) : RenderContext(w, h, dist), f_buff(f_buff) { }
    void pixel(uint16_t x, uint16_t y, Color32 c) override
    {
        f_buff[(h - y) * w + x] = c;
        //std::cout << x << ' ' << y << ' ' << (int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << '\n';
    }
private:
    uint32_t *f_buff;
};

/*
 * TGA IMAGE CONTEXT
 * */
class TgaImageContext : public RenderContext
{
public:
    TgaImage &image;
    explicit TgaImageContext(TgaImage &image, float dist) : RenderContext(image.get_width(), image.get_height(), dist), image(image) { }
    void pixel(uint16_t x, uint16_t y, Color32 c) override { image.set(x, h - y, c.bits); }
};

#endif //SIMPLE_RASTERIZER_RENDERER_H