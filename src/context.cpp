#include "context.h"

void RenderContext::points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = transform2screen(vertices[face.ivert]);
        pixel(p.x, p.y);
    }
}

void RenderContext::lines()
{
    for (size_t i = 0; i < faces.size(); i += 2)
    {
        ScreenPoint p0 = transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = transform2screen(vertices[faces[i + 1].ivert]);
        line(p0.x,p0.y, p1.x,p1.y);
    }
}

void RenderContext::triangles()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = transform2screen(vertices[faces[i + 1].ivert]);
        ScreenPoint p2 = transform2screen(vertices[faces[i + 2].ivert]);
        triangle_bound(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void RenderContext::triangles_wired()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = transform2screen(vertices[faces[i + 1].ivert]);
        ScreenPoint p2 = transform2screen(vertices[faces[i + 2].ivert]);
        triangle_wired(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void RenderContext::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ScreenLine l(x0, y0, x1, y1);
    pixel(*l.x, *l.y);
    while (l.move())
        pixel(*l.x, *l.y);
}

void RenderContext::triangle_bound(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Vec2Int a(x0, y0);
    Vec2Int b(x1, y1);
    Vec2Int c(x2, y2);

    if (x2 < x0) { std::swap(x2, x0); }
    if (x2 < x1) { std::swap(x2, x1); }
    if (x1 < x0) { std::swap(x1, x0); }
    if (y2 < y0) { std::swap(y2, y0); }
    if (y2 < y1) { std::swap(y2, y1); }
    if (y1 < y0) { std::swap(y1, y0); }

    for (uint16_t x = x0; x <= x2; x++)
    for (uint16_t y = y0; y <= y2; y++)
    {
        if (geometry::is_in_triangle(Vec2Int(x, y), a, b, c))
            pixel(x, y);
//        Vec3Float bc = geometry::barycentric(Vec2Int(x, y), a, b, c);
//        if (bc.x >= 0 && bc.y >= 0 && bc.z >= 0)
//            pixel(x, y);
    }
}

void RenderContext::triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    auto brush = [this](uint16_t x, uint16_t y0, uint16_t y1)
    {
        if (y1 < y0) std::swap(y1, y0);
        for (uint16_t y = y0; y <= y1; y++)
            pixel(x, y);
    };

    if (x2 < x0) { std::swap(x2, x0); std::swap(y2, y0); }
    if (x2 < x1) { std::swap(x2, x1); std::swap(y2, y1); }
    if (x1 < x0) { std::swap(x1, x0); std::swap(y1, y0); }

    ScreenLine ab(x0,y0, x1,y1);
    ScreenLine bc(x1,y1, x2,y2);
    ScreenLine ac(x0,y0, x2,y2);

    pixel(*ac.x, *ac.y);

    uint16_t x_prev = *ac.x;
    while (ab.move())
    {
        if (x_prev == *ab.x) continue;
        while (x_prev == *ac.x) ac.move();
        brush(*ab.x, *ab.y, *ac.y);
        x_prev = *ab.x;
    }

    while (bc.move())
    {
        if (x_prev == *bc.x) continue;
        while (x_prev == *ac.x) ac.move();
        brush(*bc.x, *bc.y, *ac.y);
        x_prev = *bc.x;
    }
}

void RenderContext::triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    line(x0,y0, x1,y1);
    line(x1,y1, x2,y2);
    line(x2,y2, x0,y0);
}

RenderContext::ScreenPoint RenderContext::transform2screen(Vec3Float v)
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scale_vector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}

RenderContext::ScreenLine::ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t dy = std::abs(y1 - y0);
    uint16_t dx = std::abs(x1 - x0);

    if (dy > dx)
    {
        std::swap(dy, dx);
        std::swap(x0, y0);
        std::swap(x1, y1);
        flipped = true;
        x = &_y;
        y = &_x;
    }
    else
    {
        flipped = false;
        x = &_x;
        y = &_y;
    }

    _2dy = (dy << 1);
    int _2dx = (dx << 1);
    dev = _2dy - dx;
    ddev = _2dy - _2dx;
    diry = y0 > y1 ? -1 : 1;
    dirx = x0 > x1 ? -1 : 1;
    _x1 = x1;
    _x = x0;
    _y = y0;
}

bool RenderContext::ScreenLine::move()
{
    if (_x == _x1)
        return false;

    _x += dirx;
    if (dev > 0)
    {
        _y += diry;
        dev += ddev;
    }
    else { dev += _2dy; }

    return true;
}
