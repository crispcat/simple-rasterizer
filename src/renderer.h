#ifndef SIMPLE_RASTERIZER_RENDERER_H
#define SIMPLE_RASTERIZER_RENDERER_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"
#include "thread_pool.h"
#include "renderer_structs.h"

class RenderContext
{

public:
    std::vector<Vec3> vertices{};
    std::vector<Vec2> uvs{};
    std::vector<Vec3> normals{};
    std::vector<Vec3Int> faces{};
    Vec3 light_dir = Vec3::back();

public:
    Color32 foregr_color = COLOR_WHITE;
    Color32 backgr_color = COLOR_BLACK;

public:
    explicit RenderContext(uint32_t *frame_buff, uint16_t width, uint16_t height);
    virtual ~RenderContext();
    void set_tex(const TgaImage &t);
    void set_buff(uint32_t *frame_buff, uint16_t width, uint16_t height);
    void set_cam(Vec3 eye, Vec3 center, Vec3 up);
    void set_viewport(uint32_t width, uint32_t height);
    void start_frame();
    void render() { start_frame(); drawcall(); }

public:
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

public:
    void drawcall();
    void triangle(std::array<Vert, 3>  v);

protected:
    void apply_texture(Frag &f) const;
    void flat_light(Frag &f) const;
    void gouroud_light(Frag &f) const;
    void fong_light(Frag &f) const;

protected:
    uint16_t w;
    uint16_t h;
    TgaImage tex;
    Vec2 tex_scale;
    DepthBufferValue *z_buff;
    uint32_t *f_buff;
    std::atomic_flag *frag_locks;
    BS::thread_pool_light t_pool;

protected:
    Matrix<4, 4, float> view;
    Matrix<4, 4, float> proj;
    Matrix<4, 4, float> viewport;

protected:
    void vert(Vert &v) const;
    void frag(Frag &f);
    virtual void pixel(uint16_t x, uint16_t y, Color32 c);

};

#endif //SIMPLE_RASTERIZER_RENDERER_H