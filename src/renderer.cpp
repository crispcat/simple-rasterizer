#include <vector>
#include "renderer.h"
#include "geometry.h"

RenderContext::RenderContext(uint32_t *frame_buff, uint16_t width, uint16_t height) :
    w(),
    h(),
    tex_scale { },
    z_buff(nullptr),
    f_buff(nullptr),
    frag_locks(nullptr),
    t_pool { } {
    set_buff(frame_buff, width, height);
};

void RenderContext::set_buff(uint32_t *frame_buff, uint16_t width, uint16_t height)
{
    w = width;
    h = height;
    f_buff = frame_buff;
    delete[] z_buff; z_buff = new float[w * h];
    delete[] frag_locks; frag_locks = new std::atomic_flag[w * h];
    for (uint32_t i = 0; i < w * h; i++) frag_locks[i].clear();
    screen_scale = { (float)w / 2, (float)h / 2, 1.f };
    set_cam({ 0.f, 0.f, 1.f }, Vec3::zero(), Vec3::up());
}

void RenderContext::set_tex(const TgaImage &t)
{
    tex_scale = Vec3((float)t.get_width(), (float)t.get_height(), 0.f);
    tex = t;
}

RenderContext::~RenderContext()
{
    delete[] z_buff;
    delete[] frag_locks;
}

void RenderContext::sta_fr()
{
    std::fill(z_buff, z_buff + w * h, -clipping_plane);
    std::fill(f_buff, f_buff + w * h, backgr_color);
}

void RenderContext::pixel(uint16_t x, uint16_t y, Color32 c)
{
    f_buff[(h - y) * w + x] = c;
}

