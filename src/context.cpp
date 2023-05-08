#include "context.h"

void primitives::line(TgaImageContext c, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

        uint16_t dy = std::abs(y1 - y0);
        uint16_t dx = std::abs(x1 - x0);

        c.flipped = false;
        if (dy > dx) {
                std::swap(dy, dx);
                std::swap(x0, y0);
                std::swap(x1, y1);
                c.flipped = true;
        }

        if (x0 > x1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
        }

        c.pixel(x0, y0);

        int32_t _2dy = (dy << 1);
        int32_t _2dx = (dx << 1);
        int32_t dev  = _2dy - dx;
        int32_t ddev = _2dy - _2dx;
        int32_t sdy  = y1 - y0;
        int32_t diry = sdy > 0 ? 1 : sdy < 0 ? -1 : 0;
        for (uint16_t x = x0 + 1, y = y0; x <= x1; x++) {
                if (dev > 0) {
                        y += diry;
                        dev += ddev;
                } else {
                        dev += _2dy;
                }
                c.pixel(x, y);
        }
}
