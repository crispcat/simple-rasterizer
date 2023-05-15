#include "context.h"

TgaImageContext::TgaImageContext(TGAImage &i) :
    tgaImage(i),
    scaleVector(Vec3Float((float)i.get_width() / 2, (float)i.get_height() / 2, 1.f)) { }

void TgaImageContext::SetImage(TGAImage &i)
{
    tgaImage = i;
    scaleVector = Vec3Float((float)i.get_width() / 2, (float)i.get_height() / 2, 1.f);
}

void TgaImageContext::points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = vertex_transform2screen(vertices[face.ivert]);
        pixel(p.x, p.y, false);
    }
}

void TgaImageContext::lines()
{
    for (size_t i = 0; i < faces.size(); i += 2)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        line(p0.x,p0.y, p1.x,p1.y);
    }
}

void TgaImageContext::triangles()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        ScreenPoint p2 = vertex_transform2screen(vertices[faces[i+2].ivert]);
        triangle_old(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void TgaImageContext::triangles_w()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        ScreenPoint p2 = vertex_transform2screen(vertices[faces[i+2].ivert]);
        w_triangle(p0.x, p0.y, p1.x,p1.y, p2.x, p2.y);
    }
}

void TgaImageContext::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ScreenLine l(x0, y0, x1, y1);
    pixel(l.x, l.y, l.flipped);
    while (l.move())
        pixel(l.x, l.y, l.flipped);
}

void TgaImageContext::triangle_old(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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

    uint16_t* _x0 = ac.flipped ? &ac.y : &ac.x;
    uint16_t* _y0 = ac.flipped ? &ac.x : &ac.y;
    uint16_t* _y1 = ab.flipped ? &ab.x : &ab.y;
    while (ab.move() && ac.move())
        brush(*_x0, *_y0, *_y1);

    _y1 = bc.flipped ? &bc.x : &bc.y;
    while (bc.move() && ac.move())
        brush(*_x0, *_y0, *_y1);
}

void TgaImageContext::w_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    line(x0,y0, x1,y1);
    line(x1,y1, x2,y2);
    line(x2,y2, x0,y0);
}

ScreenPoint TgaImageContext::vertex_transform2screen(Vec3Float v)
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scaleVector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}

void TgaImageContext::pixel(uint16_t x, uint16_t y, bool flipped)
{
    if (flipped)
        tgaImage.set(y, x, tgaColor);
    else
        tgaImage.set(x, y, tgaColor);
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

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    _2dy = (dy << 1);
    int _2dx = (dx << 1);
    dev = _2dy - dx;
    ddev = _2dy - _2dx;
    int sdy = y1 - y0;
    diry = sdy > 0 ? 1 : sdy < 0 ? -1 : 0;
    x = x0;
    y = y0;
    this->x1 = x1;
}

bool ScreenLine::move()
{
    if (x == x1)
        return false;

    x++;
    if (dev > 0)
    {
        y += diry;
        dev += ddev;
    }
    else { dev += _2dy; }

    return true;
}
