#include <iostream>
#include "tests.h"
#include "fpsmon.h"
#include "fenster.h"
#include "renderer.h"
#include "objmodel.h"
#include "fpsmon.h"

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

        Hom camera_pos(Vec3(0.f, 0.f, 1.5f));
        context.set_cam(camera_pos.proj3d(), Vec3::zero(), Vec3::up());

        Vec3 mouse_pos(fenster.x(), fenster.y(), 0.f);

        FPSMon fps_mon;
        while (fenster_loop(&fenster.f) == 0)
        {
            fps_mon.frame_start();
            context.render();
            fps_mon.frame_end();

            Vec3 new_pos(fenster.x(), fenster.y(), 1.f);

            if (fenster.mouse())
            {
                Vec3 m_delta = new_pos - mouse_pos;
                float m_dist = m_delta.norm();

                if (m_dist == 0.f)
                    continue;

                float angle = m_dist * 2 * M_PIf/w;
                Vec3 m_dir = m_delta.normalized();

                camera_pos = rotate({ m_dir.y, m_dir.x, 0.f }, -angle) * camera_pos;
                context.set_cam(camera_pos.proj3d(), Vec3::zero(), Vec3::up());
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
