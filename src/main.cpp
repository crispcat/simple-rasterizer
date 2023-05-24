#include <iostream>
#include "fenster.h"
#include "renderer.h"
#include "objmodel.h"
#include "tests.h"

const int FPS = 30;
void draw_model(RenderContext &c, const ObjModel &m);
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
        RtContext c(f.getbuff(), w, h);
        while (f.loop(FPS))
            draw_model(c, m);
    }
    else if (mode == "tga")
    {
        TgaImage i(w, h, TgaImage::Format::RGB);
        TgaImageContext c(i);
        draw_model(c, m);
        c.image.write_tga_file("output.tga");
        std::cout << "Model rendered to output.tga" << std::endl;
    }
    else if (mode == "tests")
    {
        //calc_vectors();
        calc_matrices();

        TgaImage i1(100, 100, TgaImage::Format::RGB);
        TgaImage i2(200, 200, TgaImage::Format::RGB);

        TgaImageContext c1(i1);
        draw_primitives_lines(c1);
        c1.image.write_tga_file("lines.tga");
        std::cout << "Lines test rendered to lines.tga" << std::endl;

        TgaImageContext c2(i2);
        draw_primitives_triangles(c2);
        c2.image.write_tga_file("drawcall.tga");
        std::cout << "Triangles test rendered to drawcall.tga" << std::endl;
    }

    return 0;
}

void draw_model(RenderContext &c, const ObjModel &m)
{
    c.set_texture(m.texture);
    c.vertices = m.vertices;
    c.normals = m.normals;
    c.faces = m.faces;
    c.uvs = m.uvs;
    c.drawcall();
//    color.color = COLOR_GREEN;
//    color.gizmo_triangles();
//    color.color = COLOR_RED;
//    color.gizmo_points();
}
