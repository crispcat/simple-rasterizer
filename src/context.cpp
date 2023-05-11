#include "context.h"

TgaImageContext::TgaImageContext(TGAImage &i) :
    tgaImage(i),
    scaleVector(Vec3Float((float_t)i.get_width() / 2, (float_t)i.get_height() / 2, 1.f)) { }

void TgaImageContext::SetImage(TGAImage &i)
{
    tgaImage = i;
    scaleVector = Vec3Float((float_t)i.get_width() / 2, (float_t)i.get_height() / 2, 1.f);
}

void TgaImageContext::points()
{
    for (Vec3Int &face : faces)
    {
        ScreenPoint p = vertex_transform2screen(vertices[face.ivert]);
        pixel(p.x, p.y);
    }
}

void TgaImageContext::lines()
{
    for (size_t i = 0; i < faces.size(); i += 2)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        line(p0.x, p0.y, p1.x,p1.y);
    }
}

void TgaImageContext::triangles()
{

}

void TgaImageContext::w_triangles()
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

    pixel(x0, y0);

    int32_t _2dy = (dy << 1);
    int32_t _2dx = (dx << 1);
    int32_t dev = _2dy - dx;
    int32_t ddev = _2dy - _2dx;
    int32_t sdy = y1 - y0;
    int32_t diry = sdy > 0 ? 1 : sdy < 0 ? -1 : 0;
    for (uint16_t x = x0 + 1, y = y0; x <= x1; x++)
    {
        if (dev > 0)
        {
            y += diry;
            dev += ddev;
        }
        else { dev += _2dy; }
        pixel(x, y);
    }
}

void TgaImageContext::triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

}

void TgaImageContext::w_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    line(x0,y0, x1,y1);
    line(x1,y1, x2,y2);
    line(x2,y2, x0,y0);
}

void TgaImageContext::pixel(uint16_t x, uint16_t y)
{
    if (flipped)
        tgaImage.set(y, x, tgaColor);
    else
        tgaImage.set(x, y, tgaColor);
}

ScreenPoint TgaImageContext::vertex_transform2screen(Vec3Float v)
{
    Vec3Float sv = (v + Vec3FloatOne).scale(scaleVector);
    return {(uint16_t)sv.x, (uint16_t)sv.y};
}