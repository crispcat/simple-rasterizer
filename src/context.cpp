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
        triangle_ylined(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
        std::cout << "Rendered " << i << ' ' << i + 1 << ' ' << i + 2 << '\n';
    }
}

void TgaImageContext::triangles_w()
{
    for (size_t i = 0; i < faces.size(); i += 3)
    {
        ScreenPoint p0 = vertex_transform2screen(vertices[faces[i].ivert]);
        ScreenPoint p1 = vertex_transform2screen(vertices[faces[i+1].ivert]);
        ScreenPoint p2 = vertex_transform2screen(vertices[faces[i+2].ivert]);
        triangle_wired(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

void TgaImageContext::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t dy = std::abs(y1 - y0);
    uint16_t dx = std::abs(x1 - x0);

    bool flipped = false;
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

    pixel(x0, y0, flipped);

    int _2dy = (dy << 1);
    int _2dx = (dx << 1);
    int dev = _2dy - dx;
    int ddev = _2dy - _2dx;
    int sdy = y1 - y0;
    int diry = sdy > 0 ? 1 : sdy < 0 ? -1 : 0;
    for (uint16_t x = x0 + 1, y = y0; x <= x1; x++)
    {
        if (dev > 0)
        {
            y += diry;
            dev += ddev;
        }
        else { dev += _2dy; }
        pixel(x, y, flipped);
    }
}

void TgaImageContext::triangle_xlined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

}

void TgaImageContext::triangle_ylined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (y2 < y0) { std::swap(y2, y0); std::swap(x2, x0); }
    if (y1 < y0) { std::swap(y1, y0); std::swap(x1, x0); }
    if (y2 < y1) { std::swap(y2, y1); std::swap(x2, x1); }

    uint16_t height_c = y2 - y0;
    uint16_t height_b = y2 - y1;
    uint16_t height_a = y1 - y0;

    for (uint16_t y = y0; y <= y1; y++)
    {
        float ta = height_a == 0 ? 0 : (float)(y - y0) / height_a;
        float tc = height_c == 0 ? 0 : (float)(y - y0) / height_c;
        uint16_t xa = x0 + (x1 - x0) * ta;
        uint16_t xc = x0 + (x2 - x0) * tc;
        if (xc < xa) std::swap(xc, xa);
        for (uint16_t x = xa; x <= xc; x++)
            pixel(x, y, false);
    }

    for (uint16_t y = y1; y <= y2; y++)
    {
        float tb = height_b == 0 ? 0 : (float)(y - y1) / height_b;
        float tc = height_c == 0 ? 0 : (float)(y - y0) / height_c;
        uint16_t xb = x1 + (x2 - x1) * tb;
        uint16_t xc = x0 + (x2 - x0) * tc;
        if (xc < xb) std::swap(xc, xb);
        for (uint16_t x = xb; x <= xc; x++)
            pixel(x, y, false);
    }
}

void TgaImageContext::triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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
