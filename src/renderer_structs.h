#ifndef SIMPLE_RASTERIZER_RENDERER_STRUCTS_H
#define SIMPLE_RASTERIZER_RENDERER_STRUCTS_H

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
    Color32(Vec3Int i) : b(i.b), g(i.g), r(i.r) { }
    Color32(Vec3Float f) : b(std::round(f.b)), g(std::round(f.b)), r(std::round(f.r)) { }

    operator uint32_t() const { return bits; }
    Vec3Int vec() const { return Vec3Int(r, g, b); }
    Vec3Float vecf() const { return Vec3Float(r, g, b); }
    Vec3Float norm() const { return Vec3Float(r, g, b).scale(1.f / 255); }
};

const Color32 COLOR_BLACK (0x000000);
const Color32 COLOR_RED   (0xfa0000);
const Color32 COLOR_GREEN (0x00fa00);
const Color32 COLOR_BLUE  (0x0000fa);
const Color32 COLOR_WHITE = COLOR_RED + COLOR_GREEN + COLOR_BLUE;

// a potential pixel
struct Frag
{
    // screen pixel
    ScreenPoint pix;
    // screen color
    Color32 color;
    // barycentric coordinates
    Vec3Float bcentr;
    // normal
    Vec3Float n;
    // 3D space triangle vertices
    Vec3Float *v;
    // triangle vertices screen projection
    ScreenPoint *sv;
    // vertices normals
    Vec3Float *ns;
    // vertices UVs
    Vec3Float *uvs;
    //
    Frag() = delete;
    explicit Frag(ScreenPoint &pix) : pix(std::move(pix)) { }
    explicit Frag(ScreenPoint &&pix) : pix(std::move(pix)) { }
    void set_uvs(Vec3Float (&&_uvs)[3]) { uvs = std::move(_uvs); }
    void set_ns(Vec3Float (&&_ns)[3]) { ns = std::move(_ns); }
};

struct ScreenLine
{
    uint16_t *x;
    uint16_t *y;
    bool flipped;
    uint16_t _x;
    uint16_t _y;
    uint16_t _x1;
    int dev;
    int ddev;
    int _2dy;
    int diry;
    int dirx;
    ScreenLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    bool move();
};

#endif //SIMPLE_RASTERIZER_RENDERER_STRUCTS_H