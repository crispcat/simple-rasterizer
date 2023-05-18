#include "renderer.h"

RenderContext::RenderContext(uint16_t w, uint16_t h, float distance) :
    w(w),
    h(h),
    z_max(distance),
    have_ns(false),
    have_uvs(false),
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
    float z = geometry::dot(f.bcentr, { f.v[0].z, f.v[1].z, f.v[2].z });
    uint32_t i = f.pix.y * h + f.pix.x;
    if (z > z_buff[i])
    {
        flat_light(f);
        pixel(f.pix.x, f.pix.y, f.color);
        z_buff[i] = z;
    }
}

void RenderContext::flat_light(Frag &f) const
{
    Vec3Float n = geometry::cross(f.v[2] - f.v[0], f.v[1] - f.v[0]).normalized();
    float intensity = geometry::dot(light_dir, n);
    intensity = intensity < 0 ? 0 : intensity;
    f.color = f.color.vecf().scale(intensity);
}

void RenderContext::gouroud_light(Frag &f)
{

}

ScreenPoint RenderContext::transform2screen(Vec3Float v) const
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scale_vector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}


