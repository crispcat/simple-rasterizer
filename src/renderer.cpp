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

void RenderContext::vert(Vert &v) const
{
    transform(v.pos, transformer::perspective(camera_distance));
}

void RenderContext::frag(Frag &f)
{
    auto &v = f.v;
    float z = geometry::dot(f.bcentr, { v[0].pos.z, v[1].pos.z, v[2].pos.z });
    uint32_t i = f.pix.y * h + f.pix.x;
    if (z <= z_buff[i])
        return;
    z_buff[i] = z;
    apply_texture(f);
    gouroud_light(f);
    pixel(f.pix.x, f.pix.y, f.color);
}

void RenderContext::apply_texture(Frag &f) const
{
    auto &v = f.v;
    float pu = geometry::dot(f.bcentr, { v[0].tex.u, v[1].tex.u, v[2].tex.u });
    float pv = geometry::dot(f.bcentr, { v[0].tex.v, v[1].tex.v, v[2].tex.v });
    Vec2Int tcoord = Vec2(pu, pv).scale(tex_scale).apply(std::round);
    f.color = tex.get(tcoord.u, tcoord.v).val;
}

void RenderContext::flat_light(Frag &f) const
{
    auto &v = f.v;
    Vec3 n = geometry::cross(v[2].pos - v[0].pos, v[1].pos - v[0].pos).normalized();
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

void RenderContext::gouroud_light(Frag &f) const
{
    auto &v = f.v;
    float l0 = calc::clamp0(geometry::dot(light_dir, v[0].norm));
    float l1 = calc::clamp0(geometry::dot(light_dir, v[1].norm));
    float l2 = calc::clamp0(geometry::dot(light_dir, v[2].norm));
    float l = geometry::dot(f.bcentr, { l0, l1, l2 });
    f.color = f.color.vecf().scale(l);
}

void RenderContext::fong_light(Frag &f) const
{
    auto &v = f.v;
    Vec3 n = { geometry::dot(f.bcentr, { v[0].norm.x, v[1].norm.x, v[2].norm.x }),
               geometry::dot(f.bcentr, { v[0].norm.y, v[1].norm.y, v[2].norm.y }),
               geometry::dot(f.bcentr, { v[0].norm.z, v[1].norm.z, v[2].norm.z }) };
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

void RenderContext::pixel(uint16_t x, uint16_t y, Color32 c)
{
    f_buff[(h - y) * w + x] = c;
}

ScreenPoint RenderContext::transform2screen(Vec3 v) const
{
    Vec3 sv = (v + Vec3One).scale(screen_scale).apply(std::round);
    return (ScreenPoint) sv;
}

