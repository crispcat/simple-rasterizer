#ifndef SIMPLE_RASTERIZER_CONTEXT_H
#define SIMPLE_RASTERIZER_CONTEXT_H

#include "tgaimage.h"

struct TgaImageContext {

        TGAImage& tgaImage;
        TGAColor tgaColor;
        bool flipped;

        explicit TgaImageContext(TGAImage& i) : tgaImage(i), tgaColor(), flipped(false) {}

        inline void pixel(uint16_t x, uint16_t y) {
                if (flipped) tgaImage.set(y, x, tgaColor);
                else tgaImage.set(x, y, tgaColor);
        }
};

namespace primitives {

        void line(TgaImageContext c, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

}
#endif //SIMPLE_RASTERIZER_CONTEXT_H
