#include "renderer.h"

void RenderContext::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ScreenLine l(x0, y0, x1, y1);
    pixel(l.x, l.y, color);
    while (l.move())
        pixel(l.x, l.y, color);
}

void RenderContext::triangle_lined(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    auto brush = [this](uint16_t x, uint16_t y0, uint16_t y1)
    {
        if (y1 < y0) std::swap(y1, y0);
        for (uint16_t y = y0; y <= y1; y++)
            pixel(x, y, color);
    };

    if (x2 < x0) { std::swap(x2, x0); std::swap(y2, y0); }
    if (x2 < x1) { std::swap(x2, x1); std::swap(y2, y1); }
    if (x1 < x0) { std::swap(x1, x0); std::swap(y1, y0); }

    ScreenLine ab(x0,y0, x1,y1);
    ScreenLine bc(x1,y1, x2,y2);
    ScreenLine ac(x0,y0, x2,y2);

    pixel(ac.x, ac.y, color);

    uint16_t x_prev = ac.x;
    while (ab.move())
    {
        if (x_prev == ab.x) continue;
        while (x_prev == ac.x) ac.move();
        brush(ab.x, ab.y, ac.y);
        x_prev = ab.x;
    }

    while (bc.move())
    {
        if (x_prev == bc.x) continue;
        while (x_prev == ac.x) ac.move();
        brush(bc.x, bc.y, ac.y);
        x_prev = bc.x;
    }
}

void RenderContext::triangle_wired(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    line(x0,y0, x1,y1);
    line(x1,y1, x2,y2);
    line(x2,y2, x0,y0);
}

ScreenLine::ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) : x(x0), y(y0), x1(x1), y1(y1)
{
    dy = std::abs(y1 - y0);
    dx = std::abs(x1 - x0);
    dev = (dx > dy ? dx : -dy) / 2;
    diry = y0 > y1 ? -1 : 1;
    dirx = x0 > x1 ? -1 : 1;
}

bool ScreenLine::move()
{
    if (x == x1 && y == y1)
        return false;

    int ddev = dev;
    if (ddev > -dx)
    {
        dev -= dy;
        x += dirx;
    }
    if (ddev < dy)
    {
        dev += dx;
        y += diry;
    }

    return true;
}
