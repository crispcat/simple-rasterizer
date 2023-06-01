#include <iostream>
#include "fenster.h"
#include "renderer.h"
#include "objmodel.h"
#include "tests.h"

#define FPS_MON_INIT \
    int frames = 0; \
    int64_t ttf_accum = 0;

#define FPS_MON_FRAME_START \
    int64_t f_start = fenster_time();

#define FPS_MON_FRAME_END \
    ttf_accum += fenster_time() - f_start; \
    frames++; \
    if (ttf_accum >= 1000) \
    { \
        std::cout << "FPS: " << frames << " TTF: " << ttf_accum / frames << " ms." << '\n'; \
        ttf_accum = 0; \
        frames = 0; \
    } \

void set_model(RenderContext &c, const ObjModel &m);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Arguments are ('rt'|'tga'|'tests') 'model_path' 'width' 'height' all required.\n";
        return 1;
    }

    std::string mode(argv[1]);
    std::string model_path(argv[2]);
    uint16_t w = std::stoi(argv[3]);
    uint16_t h = std::stoi(argv[4]);

    ObjModel model(model_path);

    if (mode == "rt")
    {
        Fenster fenster(w, h, "simple-rasterizer");
        RenderContext context(fenster.getbuff(), w, h);
        set_model(context, model);
        Vec3 camera_pos(0.f, 0.f, 1.5f);
        Vec3 mouse_pos(fenster.x(), fenster.y(), 0.f);
        Vec3 p_to_rads(2 * M_PIf / w, 2 * M_PIf / h, 1.f);
        Vec3 rads;
        FPS_MON_INIT
        while (fenster_loop(&fenster.f) == 0)
        {
            FPS_MON_FRAME_START
            context.render();
            FPS_MON_FRAME_END
            Vec3 new_pos(fenster.x(), fenster.y(), 1.f);
            if (fenster.mouse())
            {
                rads += (new_pos - mouse_pos).scale(p_to_rads);
                Vec3 r_camera_pos = transform(camera_pos, rotate( { -rads.y, -rads.x, 0.f } ));
                context.set_cam(r_camera_pos, Vec3::zero(), Vec3::up());
            }
            mouse_pos = new_pos;
        }
    }
    else if (mode == "tga")
    {
        TgaImage image(w, h, TgaImage::Format::RGBA);
        RenderContext context((uint32_t*)image.buffer(), w, h);
        set_model(context, model);
        context.render();
        image.write_tga_file("output.tga");
        std::cout << "Model rendered to output.tga" << std::endl;
    }
    else if (mode == "tests")
    {
        calc_vectors();
        calc_matrices();

        TgaImage image1(100, 100, TgaImage::Format::RGBA);
        TgaImage image2(200, 200, TgaImage::Format::RGBA);

        RenderContext context((uint32_t*)image1.buffer(), 100, 100);
        draw_primitives_lines(context);
        image1.write_tga_file("lines.tga");
        std::cout << "Lines test rendered to lines.tga" << std::endl;

        context.set_buff((uint32_t*) image2.buffer(), 200, 200);
        draw_primitives_triangles(context);
        image2.write_tga_file("triangles.tga");
        std::cout << "Triangles test rendered to drawcall.tga" << std::endl;
    }

    return 0;
}

void set_model(RenderContext &c, const ObjModel &m)
{
    c.set_tex(m.diffuse);
    c.vertices = m.vertices;
    c.normals = m.normals;
    c.faces = m.faces;
    c.uvs = m.uvs;
//    foregr_color.foregr_color = COLOR_GREEN;
//    foregr_color.gizmo_triangles();
//    foregr_color.foregr_color = COLOR_RED;
//    foregr_color.gizmo_points();
}
