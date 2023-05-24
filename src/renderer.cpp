#include "renderer.h"
#include "geometry.h"

RenderContext::RenderContext(uint16_t w, uint16_t h) :
    w(w),
    h(h),
    viewport_scale_vector({(float)w / 2, (float)h / 2, 1.f }),
    texture_scale_vector() {
    z_buff = new float[w * h];
    frame();
};

RenderContext::~RenderContext()
{
    delete[] z_buff;
}

void RenderContext::frame()
{
    std::fill(z_buff, z_buff + w * h, -clipping_plane);
}

void RenderContext::set_texture(const TgaImage &t)
{
    texture_scale_vector = Vec3((float)t.get_width(), (float)t.get_height(), 0.f);
    texture = t;
}

void RenderContext::vert(Vert &v) const
{
    transform(v.pos, transformer::perspective(camera_distance));
}

void RenderContext::frag(Frag &f)
{
    const Vert *v = f.v;
    float z = geometry::dot(f.bcentr, { v[0].pos.z, v[1].pos.z, v[2].pos.z });
    uint32_t i = f.pix.y * h + f.pix.x;
    if (z > z_buff[i])
    {
        apply_texture(f);
        gouroud_light(f);
        pixel(f.pix.x, f.pix.y, f.color);
        z_buff[i] = z;
    }
}

void RenderContext::apply_texture(Frag &f) const
{
    const Vert *v = f.v;
    float pu = geometry::dot(f.bcentr, { v[0].tex.u, v[1].tex.u, v[2].tex.u });
    float pv = geometry::dot(f.bcentr, { v[0].tex.v, v[1].tex.v, v[2].tex.v });
    Vec2Int tcoord = Vec2(pu, pv).scale(texture_scale_vector).apply(std::round);
    f.color = texture.get(tcoord.u, tcoord.v).val;
}

void RenderContext::flat_light(Frag &f) const
{
    const Vert *v = f.v;
    Vec3 n = geometry::cross(v[2].pos - v[0].pos, v[1].pos - v[0].pos).normalized();
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

void RenderContext::gouroud_light(Frag &f) const
{
    const Vert *v = f.v;
    float l0 = calc::clamp0(geometry::dot(light_dir, v[0].norm));
    float l1 = calc::clamp0(geometry::dot(light_dir, v[1].norm));
    float l2 = calc::clamp0(geometry::dot(light_dir, v[2].norm));
    float l = geometry::dot(f.bcentr, { l0, l1, l2 });
    f.color = f.color.vecf().scale(l);
}

void RenderContext::fong_light(Frag &f) const
{
    const Vert *v = f.v;
    Vec3 n = { geometry::dot(f.bcentr, { v[0].norm.x, v[1].norm.x, v[2].norm.x }),
               geometry::dot(f.bcentr, { v[0].norm.y, v[1].norm.y, v[2].norm.y }),
               geometry::dot(f.bcentr, { v[0].norm.z, v[1].norm.z, v[2].norm.z }) };
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

ScreenPoint RenderContext::transform2screen(Vec3 v) const
{
    Vec3 sv = (v + Vec3One).scale(viewport_scale_vector).apply(std::round);
    return (ScreenPoint) sv;
}

