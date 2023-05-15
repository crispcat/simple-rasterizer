#include <iostream>
#include "fenster.h"
#include "context.h"
#include "objmodel.h"
//#include "tests.h"

const int FPS = 30;
void draw_model(Context &c, const ObjModel &m);
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Arguments are ('rt'|'tga') 'model_path' 'width' 'height' all required.\n";
        return 1;
    }

    std::string mode(argv[1]);
    std::string model_path(argv[2]);
    uint16_t w = std::stoi(argv[3]);
    uint16_t h = std::stoi(argv[4]);

    ObjModel m(model_path);

    if (mode == "rt")
    {
        Fenster f(w, h, "simple-rasterizer");
        RTContext c(f.getbuff(), w, h);
        while (f.loop(FPS))
            draw_model(c, m);
    }
    else if (mode == "tga")
    {
        TGAImage i(w, h, TGAImage::Format::RGB);
        TGAImageContext c(i);
        draw_model(c, m);
        c.image.write_tga_file("output.tga");
        std::cout << "Model rendered to output.tga" << std::endl;
    }

    return 0;
}

void draw_model(Context &c, const ObjModel &m)
{
    c.color = COLOR_WHITE;
    c.vertices = m.vertices;
    c.faces = m.faces;
    c.triangles();
    c.color = COLOR_GREEN;
    c.triangles_wired();
    c.color = COLOR_RED;
    c.points();
}
