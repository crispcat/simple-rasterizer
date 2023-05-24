#ifndef SIMPLE_RASTERIZER_RENDERER_STRUCTS_H
#define SIMPLE_RASTERIZER_RENDERER_STRUCTS_H

struct Color32
{
    union
    {
        uint32_t bits;
        struct { uint8_t b, g, r, a; };
    };
    //
    Color32() = default;
    Color32(uint32_t bits) : bits(bits) { }
    Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : b(b), g(g), r(r), a(a) { }
    Color32(Vec3Int i) : b(i.b), g(i.g), r(i.r) { }
    Color32(Vec3 f) : b(std::round(f.b)), g(std::round(f.b)), r(std::round(f.r)) { }
    //
    operator uint32_t() const { return bits; }
    Vec3Int vec() const { return Vec3Int(r, g, b); }
    Vec3 vecf() const { return Vec3(r, g, b); }
    Vec3 norm() const { return Vec3(r, g, b).scale(1.f / 255); }
};

const Color32 COLOR_BLACK    (0x000000);
const Color32 COLOR_RED      (0xfa0000);
const Color32 COLOR_GREEN    (0x00fa00);
const Color32 COLOR_BLUE     (0x0000fa);
const Color32 COLOR_WHITE =  COLOR_RED +
                             COLOR_GREEN +
                             COLOR_BLUE;
struct Vert
{
    Vec3 pos;
    Vec3 norm;
    Vec2 tex;
    Vert(Vec3 pos, Vec3 norm, Vec2 uv) : pos(pos), norm(norm), tex(uv) { }
};

// a potential pixel
struct Frag
{
    ScreenPoint pix;
    Color32 color;
    Vec3 bcentr;
    Vert *v;
    explicit Frag(ScreenPoint &pix, Color32 color, Vec3 bcenrt, Vert vs[3]) :
        pix(pix),
        color(color),
        bcentr(bcenrt),
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