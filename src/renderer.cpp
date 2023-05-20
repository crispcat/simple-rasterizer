#include "renderer.h"
#include "geometry.h"

RenderContext::RenderContext(uint16_t w, uint16_t h, float distance) :
    w(w),
    h(h),
    z_max(distance),
    have_ns(true),
    have_uvs(true),
    scale_vector({(float)w / 2, (float)h / 2, 1.f }) {
    z_buff = new float[w * h];
    for (uint32_t i = 0; i < w * h; i++)
        z_buff[i] = -z_max;
};

RenderContext::~RenderContext()
{
    delete[] z_buff;
}

void RenderContext::frame()
{
    for (uint32_t i = 0; i < w * h; i++)
        z_buff[i] = -z_max;

    if (uvs.empty())
        have_uvs = false;

    if (normals.empty())
        have_ns = false;
}

void RenderContext::frag(Frag &f)
{
    Vertex *v = f.v;
    float z = geometry::dot(f.bcentr, { pos(v[0]).z, pos(v[1]).z, pos(v[2]).z });
    uint32_t i = f.pix.y * h + f.pix.x;
    if (z > z_buff[i])
    {
        fong_light(f);
        pixel(f.pix.x, f.pix.y, f.color);
        z_buff[i] = z;
    }
}

void RenderContext::flat_light(Frag &f) const
{
    Vertex *v = f.v;
    Vec3Float n = geometry::cross(pos(v[2]) - pos(v[0]), pos(v[1]) - pos(v[0])).normalized();
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

void RenderContext::gouroud_light(Frag &f) const
{
    Vertex *v = f.v;
    float l0 = calc::clamp0(geometry::dot(light_dir, normal(v[0])));
    float l1 = calc::clamp0(geometry::dot(light_dir, normal(v[1])));
    float l2 = calc::clamp0(geometry::dot(light_dir, normal(v[2])));
    float l = geometry::dot(f.bcentr, { l0, l1, l2 });
    f.color = f.color.vecf().scale(l);
}

void RenderContext::fong_light(Frag &f) const
{
    Vertex *v = f.v;
    Vec3Float n = { geometry::dot(f.bcentr, { normal(v[0]).x, normal(v[1]).x, normal(v[2]).x }),
                    geometry::dot(f.bcentr, { normal(v[0]).y, normal(v[1]).y, normal(v[2]).y }),
                    geometry::dot(f.bcentr, { normal(v[0]).z, normal(v[1]).z, normal(v[2]).z })};
    float l = calc::clamp0(geometry::dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

ScreenPoint RenderContext::transform2screen(Vec3Float v) const
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scale_vector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}

