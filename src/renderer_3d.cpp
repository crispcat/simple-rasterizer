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

void RenderContext::set_viewport(float w, float h)
{
    static const float d = 255.f; // depth
    viewport = {{
        w/2,  0,    0,    w/2,
        0,    h/2,  0,    h/2,
        0,    0,    d/2,  d/2,
        0,    0,    0,    1
    }};
}

void RenderContext::vert(Vert &v) const
{
    v.clip = proj * view * Hom(v.pos);
    v.viewport = viewport * v.clip;
    v.screen = v.viewport.proj3d();
}

void RenderContext::triangle(std::array<Vert, 3>  v)
{
    vert(v[0]);
    vert(v[1]);
    vert(v[2]);

    int32_t x0 = std::min(v[0].screen.x, std::min(v[1].screen.x, v[2].screen.x)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    int32_t x1 = std::max(v[0].screen.x, std::max(v[1].screen.x, v[2].screen.x)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    int32_t y0 = std::min(v[0].screen.y, std::min(v[1].screen.y, v[2].screen.y)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    int32_t y1 = std::max(v[0].screen.y, std::max(v[1].screen.y, v[2].screen.y)); // NOLINT(cppcoreguidelines-narrowing-conversions)

    for (int32_t x = x0; x <= x1; x++)
    for (int32_t y = y0; y <= y1; y++)
    {
        ScreenPoint pix(x, y);
        Vec3 bc_screen = barycentric(pix, v[0].screen, v[1].screen, v[2].screen);
        if (bc_screen[0] < 0 || bc_screen[1] < 0 || bc_screen[2] < 0)
            continue;

        Vec3 bc_clip = { bc_screen[0] / v[0].viewport.t, bc_screen[1] / v[1].viewport.t, bc_screen[2] / v[2].viewport.t };
        bc_clip = bc_clip.scale(1.f / (bc_clip.x + bc_clip.y + bc_clip.z));

        Frag f(pix, foregr_color, bc_clip, bc_screen, v);
        frag(f);
    }
}

void RenderContext::frag(Frag &f)
{
    auto &v = f.v;

    float z = dot(f.bc_clip, Vec3(v[0].clip.z, v[1].clip.z, v[2].clip.z));
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

    float pu = dot(f.bc_clip, { v[0].tex.u, v[1].tex.u, v[2].tex.u });
    float pv = dot(f.bc_clip, { v[0].tex.v, v[1].tex.v, v[2].tex.v });

    Vec2Int tcoord = Vec2(pu, pv).scale(tex_scale).apply(std::round);
    f.color = tex.get(tcoord.u, tcoord.v).val;
}

void RenderContext::gouroud_light(Frag &f) const
{
    auto &v = f.v;

    float l0 = clamp0(dot(light_dir, v[0].norm));
    float l1 = clamp0(dot(light_dir, v[1].norm));
    float l2 = clamp0(dot(light_dir, v[2].norm));

    float l  = dot(f.bc_clip, { l0, l1, l2 });
    f.color = f.color.vecf().scale(l);
}
