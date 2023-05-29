#include <vector>
#include "renderer.h"
#include "geometry.h"

RenderContext::RenderContext(uint32_t *frame_buff, uint16_t width, uint16_t height) :
    w(),
    h(),
    tex_scale { },
    z_buff(nullptr),
    f_buff(nullptr) {
    set_buff(frame_buff, width, height);
};

void RenderContext::set_buff(uint32_t *frame_buff, uint16_t width, uint16_t height)
{
    w = width;
    h = height;
    f_buff = frame_buff;
    delete[] z_buff; z_buff = new float[w * h];
    screen_scale = {(float)w / 2, (float)h / 2, 1.f };
}

void RenderContext::set_tex(const TgaImage &t)
{
    tex_scale = Vec3((float)t.get_width(), (float)t.get_height(), 0.f);
    tex = t;
}

RenderContext::~RenderContext()
{
    delete[] z_buff;
}

void RenderContext::frame()
{
    std::fill(f_buff, f_buff + w * h, backgr_color);
    std::fill(z_buff, z_buff + w * h, -clipping_plane);
}

void RenderContext::pixel(uint16_t x, uint16_t y, Color32 c)
{
    f_buff[(h - y) * w + x] = c;
}

