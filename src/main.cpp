#include <iostream>
#include "context.h"

const auto IMAGE_W = 100;
const auto IMAGE_H = 100;

const TGAColor COLOR_WHITE = TGAColor(255, 255, 255, 255);
const TGAColor COLOR_RED   = TGAColor(255, 0,   0,   255);
const TGAColor COLOR_GREEN = TGAColor(0,   255, 0,   255);
const TGAColor COLOR_BLUE  = TGAColor(0,   0,   255, 255);
const TGAColor COLOR_BLACK = TGAColor(0,   0,   0,   255);

int main() {

        TGAImage image(IMAGE_W, IMAGE_H, TGAImage::Format::RGB);
        TgaImageContext c(image);

#ifdef PROFILE
        for (uint32_t i = 0; i < 100000; i++) {
#endif

        c.tgaColor = COLOR_GREEN;
        primitives::line(c, 50,50, 75,75);
        primitives::line(c, 50,50, 75,25);
        primitives::line(c, 50,50, 25,25);
        primitives::line(c, 50,50, 25,75);

        c.tgaColor = COLOR_RED;
        primitives::line(c, 50,50, 75,85);
        primitives::line(c, 50,50, 75,65);
        primitives::line(c, 50,50, 75,35);
        primitives::line(c, 50,50, 75,15);
        primitives::line(c, 50,50, 25,15);
        primitives::line(c, 50,50, 25,35);
        primitives::line(c, 50,50, 25,65);
        primitives::line(c, 50,50, 25,85);

        c.tgaColor = COLOR_WHITE;
        primitives::line(c, 50,50, 75,50);
        primitives::line(c, 50,50, 50,25);
        primitives::line(c, 50,50, 25,50);
        primitives::line(c, 50,50, 50,75);

#ifdef PROFILE
        }
#endif

        image.flip_vertically();
        image.write_tga_file("output.tga");
        std::cout << "Rendered!" << std::endl;

        return 0;
}
