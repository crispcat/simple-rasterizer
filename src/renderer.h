#ifndef SIMPLE_RASTERIZER_RENDERER_H
#define SIMPLE_RASTERIZER_RENDERER_H

#include <vector>
#include <memory>
#include "geometry.h"
#include "tgaimage.h"
#include "renderer_structs.h"
#include "thread_pool.h"

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
    float camera_distance = 2.f;  // from origin
    float clipping_plane = 3.f;   // far

public:
    explicit RenderContext(uint32_t *screen_buff, uint16_t width, uint16_t height);
    virtual ~RenderContext();
    void set_tex(const TgaImage &t);
    void set_s_buff(uint32_t *screen_buff, uint16_t width, uint16_t height);
    void frame();
    void flush();
    void render() { frame(); drawcall(); flush(); }
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
    void drawcall();
    void gizmo_points();
    void gizmo_triangles();
    void triangle(std::array<Vert, 3>  vs);
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
    TgaImage tex;
    Vec2 tex_scale;
    Vec3 screen_scale;
    float *z_buff;
    uint32_t *f_buff;
    uint32_t *s_buff;
    BS::thread_pool_light t_pool;
protected:
    void vert(Vert &v) const;
    void frag(Frag &f);
    virtual void pixel(uint16_t x, uint16_t y, Color32 c);
    ScreenPoint transform2screen(Vec3 v) const;
};

#endif //SIMPLE_RASTERIZER_RENDERER_H