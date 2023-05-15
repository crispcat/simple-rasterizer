#include "context.h"

/*
 * REALTIME CONTEXT
 * */
RtContext::RtContext(uint32_t *f_buff, uint16_t w, uint16_t h) :
        RenderContext(Vec3Float((float)w / 2, (float)h / 2, 1.f)),
        f_buff(f_buff),
        w(w),
        h(h)
        { }

void RtContext::pixel(uint16_t x, uint16_t y, bool flipped)
{
    if (flipped)
        f_buff[(h - x) * w + y] = color;
    else
        f_buff[(h - y) * w + x] = color;
}

/*
 * TGA IMAGE CONTEXT
 * */
TgaImageContext::TgaImageContext(TgaImage &image) :
        RenderContext(Vec3Float((float)image.get_width() / 2, (float)image.get_height() / 2, 1.f)),
        image(image),
        h(image.get_height())
    { }

void TgaImageContext::pixel(uint16_t x, uint16_t y, bool flipped)
{
    if (flipped)
        image.set(y, h - x, color);
    else
        image.set(x, h - y, color);
}

/*
 * GENERAL CONTEXT RASTERIZATION
 * */

void RenderContext::points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = vertex_transform2screen(vertices[face.ivert]);
        pixel(p.x, p.y, false);
    }
}

void RenderContext::lines()
{
    for (size_t i = 0; i < faces.size(); i += 2)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        line(p0.x,p0.y, p1.x,p1.y);
    }
}

void RenderContext::triangles()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        ScreenPoint p2 = vertex_transform2screen(vertices[faces[i+2].ivert]);
        triangle_lined(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void RenderContext::triangles_wired()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        ScreenPoint p2 = vertex_transform2screen(vertices[faces[i+2].ivert]);
        triangle_wired(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void RenderContext::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ScreenLine l(x0, y0, x1, y1);
    pixel(l.x, l.y, l.flipped);
    while (l.move())
        pixel(l.x, l.y, l.flipped);
}

void RenderContext::triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    auto brush = [this](uint16_t x, uint16_t y0, uint16_t y1)
    {
        if (y1 < y0) std::swap(y1, y0);
        for (uint16_t y = y0; y <= y1; y++)
            pixel(x, y, false);
    };

    if (x2 < x0) { std::swap(x2, x0); std::swap(y2, y0); }
    if (x2 < x1) { std::swap(x2, x1); std::swap(y2, y1); }
    if (x1 < x0) { std::swap(x1, x0); std::swap(y1, y0); }

    ScreenLine ab(x0,y0, x1,y1);
    ScreenLine bc(x1,y1, x2,y2);
    ScreenLine ac(x0,y0, x2,y2);

    pixel(ac.x, ac.y, ac.flipped);

    uint16_t* _x0 = ab.flipped ? &ab.y : &ab.x;
    uint16_t* _y0 = ab.flipped ? &ab.x : &ab.y;
    uint16_t* _x1 = ac.flipped ? &ac.y : &ac.x;
    uint16_t* _y1 = ac.flipped ? &ac.x : &ac.y;

    uint16_t _x_prev = *_x0;
    while (ab.move())
    {
        if (_x_prev == *_x0) continue;
        while (_x_prev == *_x1) ac.move();
        brush(*_x0, *_y0, *_y1);
        _x_prev = *_x0;
    }

    _x0 = bc.flipped ? &bc.y : &bc.x;
    _y0 = bc.flipped ? &bc.x : &bc.y;
    while (bc.move())
    {
        if (_x_prev == *_x0) continue;
        while (_x_prev == *_x1) ac.move();
        brush(*_x0, *_y0, *_y1);
        _x_prev = *_x0;
    }
}

void RenderContext::triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    line(x0,y0, x1,y1);
    line(x1,y1, x2,y2);
    line(x2,y2, x0,y0);
}

ScreenPoint RenderContext::vertex_transform2screen(Vec3Float v)
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scale_vector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}

ScreenLine::ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t dy = std::abs(y1 - y0);
    uint16_t dx = std::abs(x1 - x0);

    flipped = false;
    if (dy > dx)
    {
        std::swap(dy, dx);
        std::swap(x0, y0);
        std::swap(x1, y1);
        flipped = true;
    }

    _2dy = (dy << 1);
    int _2dx = (dx << 1);
    dev = _2dy - dx;
    ddev = _2dy - _2dx;
    int sdy = y1 - y0;
    diry = sdy > 0 ? 1 : sdy < 0 ? -1 : 0;
    xstep = x0 > x1 ? -1 : 1;
    x = x0;
    y = y0;
    this->x1 = x1;
}

bool ScreenLine::move()
{
    if (x == x1)
        return false;

    x += xstep;
    if (dev > 0)
    {
        y += diry;
        dev += ddev;
    }
    else { dev += _2dy; }

    return true;
}
