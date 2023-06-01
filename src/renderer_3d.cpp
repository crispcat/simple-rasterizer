#include <array>
#include "renderer.h"

void RenderContext::drawcall()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        std::array<Vert, 3> vs { Vert(vertices[faces[i].iv],   normals[faces[i].in],   uvs[faces[i].iuv]),
                                 Vert(vertices[faces[i+1].iv], normals[faces[i+1].in], uvs[faces[i+1].iuv]),
                                 Vert(vertices[faces[i+2].iv], normals[faces[i+2].in], uvs[faces[i+2].iuv]) };
        t_pool.push_task(&RenderContext::triangle, this, vs);
    }
    t_pool.wait_for_tasks();
}

void RenderContext::set_cam(Vec3 eye, Vec3 center, Vec3 up)
{
    Vec3 z = (eye - center).normalized();
    Vec3 x = cross(up, z).normalized();
    Vec3 y = cross(z, x).normalized();
    auto base = Matrix<4, 4, float>::identity();
    auto tran = Matrix<4, 4, float>::identity();
    for (int i = 0; i < 3; i++)
    {
        base[0][i] = x[i];
        base[1][i] = y[i];
        base[2][i] = z[i];
        tran[i][3] = -eye[i];
    }
    view = base * tran;
    proj = perspective((eye - center).norm());
}

void RenderContext::triangle(std::array<Vert, 3>  vs)
{
    vert(vs[0]);
    vert(vs[1]);
    vert(vs[2]);

    Vec2Int svs[3] = { transform2screen(vs[0].pos),
                       transform2screen(vs[1].pos),
                       transform2screen(vs[2].pos) };
    // box
    uint32_t x0 = std::min(svs[0].x, std::min(svs[1].x, svs[2].x));
    uint32_t x1 = std::max(svs[0].x, std::max(svs[1].x, svs[2].x));
    uint32_t y0 = std::min(svs[0].y, std::min(svs[1].y, svs[2].y));
    uint32_t y1 = std::max(svs[0].y, std::max(svs[1].y, svs[2].y));

    for (uint32_t x = x0; x <= x1; x++)
    for (uint32_t y = y0; y <= y1; y++)
    {
        if (x <= 0 || y <= 0 || x >= w || y >= h) continue;
        ScreenPoint pix(x, y);
        Vec3 bcentr = barycentric_screen(pix, svs[0], svs[1], svs[2]);
        if (bcentr.x < 0 || bcentr.y < 0 || bcentr.z < 0) continue;
        Frag f(pix, foregr_color, bcentr, vs);
        frag(f);
    }
}

void RenderContext::vert(Vert &v) const
{
    v.pos = transform(v.pos, proj * view);
}

void RenderContext::frag(Frag &f)
{
    auto &v = f.v;
    float z = dot(f.bcentr, { v[0].pos.z, v[1].pos.z, v[2].pos.z });
    uint32_t fi = f.pix.y * h + f.pix.x;
    while (frag_locks[fi].test_and_set()) ;
    if (z <= z_buff[fi]) { frag_locks[fi].clear(); return; }
    z_buff[fi] = z;
    apply_texture(f);
    gouroud_light(f);
    pixel(f.pix.x, f.pix.y, f.color);
    frag_locks[fi].clear();
}

void RenderContext::apply_texture(Frag &f) const
{
    auto &v = f.v;
    float pu = dot(f.bcentr, { v[0].tex.u, v[1].tex.u, v[2].tex.u });
    float pv = dot(f.bcentr, { v[0].tex.v, v[1].tex.v, v[2].tex.v });
    Vec2Int tcoord = Vec2(pu, pv).scale(tex_scale).apply(std::round);
    f.color = tex.get(tcoord.u, tcoord.v).val;
}

void RenderContext::flat_light(Frag &f) const
{
    auto &v = f.v;
    Vec3 n = cross(v[2].pos - v[0].pos, v[1].pos - v[0].pos).normalized();
    float l = clamp0(dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

void RenderContext::gouroud_light(Frag &f) const
{
    auto &v = f.v;
    float l0 = clamp0(dot(light_dir, v[0].norm));
    float l1 = clamp0(dot(light_dir, v[1].norm));
    float l2 = clamp0(dot(light_dir, v[2].norm));
    float l  = dot(f.bcentr, { l0, l1, l2 });
    f.color = f.color.vecf().scale(l);
}

void RenderContext::fong_light(Frag &f) const
{
    auto &v = f.v;
    Vec3 n = { dot(f.bcentr, { v[0].norm.x, v[1].norm.x, v[2].norm.x }),
               dot(f.bcentr, { v[0].norm.y, v[1].norm.y, v[2].norm.y }),
               dot(f.bcentr, { v[0].norm.z, v[1].norm.z, v[2].norm.z }) };
    float l = clamp0(dot(light_dir, n));
    f.color = f.color.vecf().scale(l);
}

Vec2Int RenderContext::transform2screen(Vec3 v) const
{
    Vec3 sv = (v + Vec3::one()).scale(screen_scale).apply(std::round);
    return (Vec2Int) sv;
}
