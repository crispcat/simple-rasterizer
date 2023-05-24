#include "renderer.h"

void RenderContext::drawcall()
{
    frame();
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        Vert v[3] = { Vert(vertices[faces[i].iv],   normals[faces[i].in],   uvs[faces[i].iuv]),
                      Vert(vertices[faces[i+1].iv], normals[faces[i+1].in], uvs[faces[i+1].iuv]),
                      Vert(vertices[faces[i+2].iv], normals[faces[i+2].in], uvs[faces[i+2].iuv]), };
        triangle(v);
    }
}

void RenderContext::triangle(Vert vs[3])
{
    vert(vs[0]);
    vert(vs[1]);
    vert(vs[2]);

    ScreenPoint svs[3] = { transform2screen(vs[0].pos),
                           transform2screen(vs[1].pos),
                           transform2screen(vs[2].pos) };
    // box
    uint16_t x0 = std::min(svs[0].x, std::min(svs[1].x, svs[2].x));
    uint16_t x1 = std::max(svs[0].x, std::max(svs[1].x, svs[2].x));
    uint16_t y0 = std::min(svs[0].y, std::min(svs[1].y, svs[2].y));
    uint16_t y1 = std::max(svs[0].y, std::max(svs[1].y, svs[2].y));

    for (uint16_t x = x0; x <= x1; x++)
    for (uint16_t y = y0; y <= y1; y++)
    {
        ScreenPoint pix(x, y);
        Vec3 bcentr = geometry::barycentric_screen(pix, svs[0], svs[1], svs[2]);
        if (bcentr.x < 0 || bcentr.y < 0 || bcentr.z < 0)
            continue;

        Frag f(pix, color, bcentr, vs);
        frag(f);
    }
}

void RenderContext::gizmo_points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = transform2screen(vertices[face.iv]);
        pixel(p.x, p.y, color);
    }
}

void RenderContext::gizmo_triangles()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = transform2screen(vertices[faces[i].iv]);
        ScreenPoint p1 = transform2screen(vertices[faces[i + 1].iv]);
        ScreenPoint p2 = transform2screen(vertices[faces[i + 2].iv]);
        triangle_wired(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}