#ifndef SIMPLE_RASTERIZER_RENDERER_STRUCTS_H
#define SIMPLE_RASTERIZER_RENDERER_STRUCTS_H

#include <array>

using DepthBufferValue = std::uint8_t;
static constexpr DepthBufferValue DEPTH_BUFFER_RESOLUTION = std::numeric_limits<DepthBufferValue>::max();

struct Color32
{
    union
    {
        uint32_t bits;
        struct { uint8_t b, g, r, a; };
    };

    Color32() = default;
    Color32(uint32_t bits) : bits(bits) { }

    Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : b(b), g(g), r(r), a(a) { }

    Color32(Vec3Int i) : b(i.b), g(i.g), r(i.r), a(0xff) { }

    Color32(Vec3 f) : b(std::round(f.b)), g(std::round(f.g)), r(std::round(f.r)), a(0xff) { }

    operator uint32_t() const { return bits; }

    Vec3Int vec() const { return Vec3Int(r, g, b); }
    Vec3 vecf() const { return Vec3(r, g, b); }
    Vec3 norm() const { return Vec3(r, g, b) / 255; }
};

const Color32 COLOR_BLACK    (0xff'00'00'00);
const Color32 COLOR_RED      (0xff'fa'00'00);
const Color32 COLOR_GREEN    (0xff'00'fa'00);
const Color32 COLOR_BLUE     (0xff'00'00'fa);
const Color32 COLOR_WHITE =  COLOR_RED +
                             COLOR_GREEN +
                             COLOR_BLUE;
struct Vert
{
    Vec3 pos;
    Vec3 norm;
    Vec2 tex;
    Hom  clip   {};
    Hom  viewport  {};
    ScreenPoint screen {};
    Vert(Vec3 pos, Vec3 norm, Vec2 uv) : pos(pos), norm(norm), tex(uv) { }
};

// a potential pixel
struct Frag
{
    ScreenPoint pix;
    Color32 color;
    Vec3 bc_viewport;
    std::array<Vert, 3> v;

    Frag(ScreenPoint pix, Vec3 barycentric, std::array<Vert, 3> vs) :
        pix(pix),
        color(COLOR_WHITE),
        bc_viewport(barycentric),
        v(vs) {
    }

};

struct ScreenLine
{
    uint16_t x;
    uint16_t y;
    uint16_t x1;
    uint16_t y1;
    int dev;
    int dy;
    int dx;
    int dirx;
    int diry;
    ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool move();
};

#endif //SIMPLE_RASTERIZER_RENDERER_STRUCTS_H