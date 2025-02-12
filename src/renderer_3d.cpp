#include <array>
#include "renderer.h"
#include "math/transform.h"
#include "math/scalar.h"

void RenderContext::set_cam(Vec3 eye, Vec3 center, Vec3 up)
{
    Vec3 z = (eye - center).normalized();
    Vec3 x = vector::cross(up, z).normalized();
    Vec3 y = vector::cross(z, x).normalized();
    persp = transform::perspective((eye - center).norm());
    view  = Projection {{
        x.x, y.x, z.x, -eye.x,
        x.y, y.y, z.y, -eye.y,
        x.z, y.z, z.z, -eye.z,
          0,   0,   0,    1
    }};
}

void RenderContext::set_viewport(uint32_t width, uint32_t height)
{
    static const uint8_t &d = DEPTH_BUFFER_RESOLUTION;
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);
    viewport = Projection {{
        w/2,  0,    0,    w/2,
        0,    h/2,  0,    h/2,
        0,    0,    d/2,  d/2,
        0,    0,    0,    1
    }};
}

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

void RenderContext::triangle(std::array<Vert, 3>  v)
{
    vert(v[0]);
    vert(v[1]);
    vert(v[2]);

    uint16_t x0 = std::min(v[0].screen.x, std::min(v[1].screen.x, v[2].screen.x));
    uint16_t x1 = std::max(v[0].screen.x, std::max(v[1].screen.x, v[2].screen.x));
    uint16_t y0 = std::min(v[0].screen.y, std::min(v[1].screen.y, v[2].screen.y));
    uint16_t y1 = std::max(v[0].screen.y, std::max(v[1].screen.y, v[2].screen.y));

    for (uint16_t x = x0; x <= x1; x++)
    for (uint16_t y = y0; y <= y1; y++)
    {
        Vec2Int pix(x, y);
        Vec3 bc_screen = vector::barycentric2(pix, v[0].screen, v[1].screen, v[2].screen);
        if (bc_screen[0] < 0 || bc_screen[1] < 0 || bc_screen[2] < 0)
            continue;

        // perspective-aware interpolation
        Vec3 bc_viewport = { bc_screen[0] / v[0].viewport.t, bc_screen[1] / v[1].viewport.t, bc_screen[2] / v[2].viewport.t };
        bc_viewport = bc_viewport * (1.f / (bc_viewport.x + bc_viewport.y + bc_viewport.z));

        Frag f(pix, bc_viewport, v);
        frag(f);
    }
}

void RenderContext::vert(Vert &v) const
{
    v.clip = persp * view * transform::embed(v.pos);
    v.viewport = viewport * v.clip;
    v.screen = transform::proj3d(v.viewport);
}

void RenderContext::frag(Frag &f)
{
    auto &v = f.v;

    DepthBufferValue z = vector::dot(f.bc_viewport, Vec3(v[0].viewport.z, v[1].viewport.z, v[2].viewport.z));
    uint32_t fi = f.pix.y * h + f.pix.x;

    while (frag_locks[fi].test_and_set()) { }
    if (z <= z_buff[fi])
    {
        frag_locks[fi].clear();
        return;
    }

    z_buff[fi] = z;
    apply_texture(f);
    gouroud_light(f);
    pixel(f.pix.x, f.pix.y, f.color);

    frag_locks[fi].clear();
}

void RenderContext::apply_texture(Frag &f) const
{
    auto &v = f.v;

    float pu = vector::dot(f.bc_viewport, { v[0].tex.u, v[1].tex.u, v[2].tex.u });
    float pv = vector::dot(f.bc_viewport, { v[0].tex.v, v[1].tex.v, v[2].tex.v });

    Vec2Int tcoord = Vec2(std::round(pu * tex_scale.x), std::round(pv * tex_scale.y));
    f.color = tex.get(tcoord.u, tcoord.v).val;
}

void RenderContext::gouroud_light(Frag &f) const
{
    auto &v = f.v;

    float l0 = scal::clamp0(vector::dot(light_dir, v[0].norm));
    float l1 = scal::clamp0(vector::dot(light_dir, v[1].norm));
    float l2 = scal::clamp0(vector::dot(light_dir, v[2].norm));

    float l  = vector::dot(f.bc_viewport, { l0, l1, l2 });
    f.color = f.color * l;
}
