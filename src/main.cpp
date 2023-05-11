#include <iostream>
#include "context.h"
#include "tgaimage.h"
#include "geometry.h"
#include "objmodel.h"

const TGAColor COLOR_WHITE = TGAColor(255, 255, 255, 255);
const TGAColor COLOR_RED   = TGAColor(255, 0,   0,   255);
const TGAColor COLOR_GREEN = TGAColor(0,   255, 0,   255);
const TGAColor COLOR_BLUE  = TGAColor(0,   0,   255, 255);
const TGAColor COLOR_BLACK = TGAColor(0,   0,   0,   255);

void drawTunnelStar(uint16_t w, uint16_t h, std::string &model);
void defileVectors(uint16_t w, uint16_t h, std::string &model);
void drawModel(uint16_t w, uint16_t h, std::string &model);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Arguments are 'model_path' 'width' 'height' all required.";
        return 1;
    }

    std::string modelPath(argv[1]);
    uint16_t w = std::stoi(argv[2]);
    uint16_t h = std::stoi(argv[3]);
    drawTunnelStar(w, h, modelPath);

    return 0;
}

void drawModel(uint16_t w, uint16_t h, std::string &model)
{
    TGAImage image(w, h, TGAImage::Format::RGB);
    TgaImageContext context(image);
    ObjModel m(model);

    context.tgaColor = COLOR_WHITE;
    context.vertices = m.vertices;
    context.faces = m.faces;
    context.w_triangles();

    context.tgaImage.flip_vertically();
    context.tgaImage.write_tga_file("output.tga");
    std::cout << "ObjModel rendered!" << std::endl;
}

void drawTunnelStar(uint16_t w, uint16_t h, std::string &model)
{
    h = 100;
    w = 100;
    TGAImage image(w, h, TGAImage::Format::RGB);
    TgaImageContext context(image);

    #ifdef PROFILE
    for (uint32_t i = 0; i < 100000; i++) {
    #endif
    context.tgaColor = COLOR_GREEN;
    context.line(50, 50, 75, 75);
    context.line(50, 50, 75, 25);
    context.line(50, 50, 25, 25);
    context.line(50, 50, 25, 75);

    context.tgaColor = COLOR_RED;
    context.line(50, 50, 75, 85);
    context.line(50, 50, 75, 65);
    context.line(50, 50, 75, 35);
    context.line(50, 50, 75, 15);
    context.line(50, 50, 25, 15);
    context.line(50, 50, 25, 35);
    context.line(50, 50, 25, 65);
    context.line(50, 50, 25, 85);

    context.tgaColor = COLOR_WHITE;
    context.line(50, 50, 75, 50);
    context.line(50, 50, 50, 25);
    context.line(50, 50, 25, 50);
    context.line(50, 50, 50, 75);
    #ifdef PROFILE
    }
    #endif

    context.tgaImage.flip_vertically();
    context.tgaImage.write_tga_file("output.tga");
    std::cout << "Tunnel Star rendered!" << std::endl;
}

void defileVectors(uint16_t w, uint16_t h, std::string &model)
{
    #ifdef PROFILE
    std::cout.setstate(std::ios_base::failbit);
    for (uint32_t i = 0; i < 100000; i++) {
    #endif
    Vec2Int vec2intA(1, -2);
    Vec2Int vec2intB(-4, 3);
    std::cout << "Vec2Int:\n";
    std::cout << vec2intA << " + " << vec2intB << " = " << vec2intA + vec2intB << '\n';
    std::cout << vec2intA << " - " << vec2intB << " = " << vec2intA - vec2intB << '\n';
    std::cout << vec2intA << " dot " << vec2intB << " = " << geometry::dot(vec2intA, vec2intB) << '\n';
    std::cout << "norm" << vec2intA << " = " << vec2intA.norm() << '\n';
    std::cout << "sqnorm" << vec2intA << " = " << vec2intA.sqnorm() << '\n';
    std::cout << "normalized" << vec2intA << " = " << vec2intA.normalized() << '\n';
    std::cout << "scale -5" << vec2intA << " = " << vec2intA.scale(-5) << '\n';

    Vec2Float vec2floatA(1.744, -2.33);
    Vec2Float vec2floatB(-4.0, 3.5);
    std::cout << "Vec2Float:\n";
    std::cout << vec2floatA << " + " << vec2floatB << " = " << vec2floatA + vec2floatB << '\n';
    std::cout << vec2floatA << " - " << vec2floatB << " = " << vec2floatA - vec2floatB << '\n';
    std::cout << vec2floatA << " dot " << vec2floatB << " = " << geometry::dot(vec2floatA, vec2floatB) << '\n';
    std::cout << "norm" << vec2floatA << " = " << vec2floatA.norm() << '\n';
    std::cout << "sqnorm" << vec2floatA << " = " << vec2floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec2floatA << " = " << vec2floatA.normalized() << '\n';
    std::cout << "scale -5" << vec2floatA << " = " << vec2floatA.scale(-5) << '\n';

    Vec3Int vec3intA(1, -2, 8);
    Vec3Int vec3intB(-4, 3, -1);
    std::cout << "Vec3Int:\n";
    std::cout << vec3intA << " + " << vec3intB << " = " << vec3intA + vec3intB << '\n';
    std::cout << vec3intA << " - " << vec3intB << " = " << vec3intA - vec3intB << '\n';
    std::cout << vec3intA << " dot " << vec3intB << " = " << geometry::dot(vec3intA, vec3intB) << '\n';
    std::cout << vec3intA << " cross " << vec3intB << " = " << geometry::cross(vec3intA, vec3intB) << '\n';
    std::cout << "norm" << vec3intA << " = " << vec3intA.norm() << '\n';
    std::cout << "sqnorm" << vec3intA << " = " << vec3intA.sqnorm() << '\n';
    std::cout << "normalized" << vec3intA << " = " << vec3intA.normalized() << '\n';
    std::cout << "scale -5" << vec3intA << " = " << vec3intA.scale(-5) << '\n';

    Vec3Float vec3floatA(1.744, -2.33, 8.3333333);
    Vec3Float vec3floatB(-4.0, 3.5, -1.91);
    std::cout << "Vec3Float:\n";
    std::cout << vec3floatA << " + " << vec3floatB << " = " << vec3floatA + vec3floatB << '\n';
    std::cout << vec3floatA << " - " << vec3floatB << " = " << vec3floatA - vec3floatB << '\n';
    std::cout << vec3floatA << " dot " << vec3floatB << " = " << geometry::dot(vec3floatA, vec3floatB) << '\n';
    std::cout << vec3floatA << " cross " << vec3floatB << " = " << geometry::cross(vec3floatA, vec3floatB) << '\n';
    std::cout << "norm" << vec3floatA << " = " << vec3floatA.norm() << '\n';
    std::cout << "sqnorm" << vec3floatA << " = " << vec3floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec3floatA << " = " << vec3floatA.normalized() << '\n';
    std::cout << "scale -5" << vec3floatA << " = " << vec3floatA.scale(-5) << '\n';
    #ifdef PROFILE
    }
    #endif
}
