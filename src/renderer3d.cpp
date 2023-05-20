#include "renderer.h"

void RenderContext::points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = transform2screen(vertices[face.iv]);
        pixel(p.x, p.y, fallback_color);
    }
}

void RenderContext::triangles_wired()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = transform2screen(vertices[faces[i].iv]);
        ScreenPoint p1 = transform2screen(vertices[faces[i + 1].iv]);
        ScreenPoint p2 = transform2screen(vertices[faces[i + 2].iv]);
        triangle_wired(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void RenderContext::triangles()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        Vertex v[3] = { faces[i], faces[i + 1], faces[i + 2] };
        triangle(v);
    }
}

void RenderContext::triangle(Vertex v[3])
{
    ScreenPoint sv[3] = { transform2screen(pos(v[0])),
                          transform2screen(pos(v[1])),
                          transform2screen(pos(v[2])) };
    // box
    uint16_t x0 = std::min(sv[0].x, std::min(sv[1].x, sv[2].x));
    uint16_t x1 = std::max(sv[0].x, std::max(sv[1].x, sv[2].x));
    uint16_t y0 = std::min(sv[0].y, std::min(sv[1].y, sv[2].y));
    uint16_t y1 = std::max(sv[0].y, std::max(sv[1].y, sv[2].y));

    for (uint16_t x = x0; x <= x1; x++)
    for (uint16_t y = y0; y <= y1; y++)
    {
        ScreenPoint pix(x, y);
        Vec3Float bcentr = geometry::barycentric_screen(pix, sv[0], sv[1], sv[2]);
        if (bcentr.x < 0 || bcentr.y < 0 || bcentr.z < 0)
            continue;

        Frag f(pix);
        f.v = v;
        f.color = fallback_color;
        f.bcentr = bcentr;
        frag(f);
    }
}