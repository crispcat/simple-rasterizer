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
    std::vector<Vec3> vertices{};
    std::vector<Vec2> uvs{};
    std::vector<Vec3> normals{};
    std::vector<Vec3Int> faces{};
    Color32 color = COLOR_WHITE;
    Vec3 light_dir { 0.f, 0.f, -1.f };
    float camera_distance = 1.5f;
    float clipping_plane = 3.f;

public:
    explicit RenderContext(uint16_t w, uint16_t h);
    virtual ~RenderContext();
    void set_texture(const TgaImage &t);
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
    void drawcall();
    void gizmo_points();
    void gizmo_triangles();
protected:
    void triangle(Vert vs[3]);
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
    float *z_buff;
    void vert(Vert &v) const;
    void frag(Frag &f);
    virtual void pixel(uint16_t x, uint16_t y, Color32 c) = 0;
    Vec3 viewport_scale_vector;
    ScreenPoint transform2screen(Vec3 v) const;
    TgaImage texture;
    Vec2 texture_scale_vector;
};

/*
 * REALTIME CONTEXT
 * */
class RtContext : public RenderContext
{
public:
    explicit RtContext(uint32_t *f_buff, uint16_t w, uint16_t h) : RenderContext(w, h), f_buff(f_buff) { }
    void pixel(uint16_t x, uint16_t y, Color32 c) override { f_buff[(h - y) * w + x] = c; }
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
    explicit TgaImageContext(TgaImage &image) : RenderContext(image.get_width(), image.get_height()), image(image) { }
    void pixel(uint16_t x, uint16_t y, Color32 c) override { image.set(x, h - y, c.bits); }
};

#endif //SIMPLE_RASTERIZER_RENDERER_H