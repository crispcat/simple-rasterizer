#ifndef SIMPLE_RASTERIZER_GEOMETRY_H
#define SIMPLE_RASTERIZER_GEOMETRY_H

#include <cmath>
#include <iostream>

template<class T> struct Vector2
{
    union
    {
        struct{ T x, y; };
        struct{ T u, v; };
        T raw[2];
    };

    Vector2() = default;
    Vector2(T x, T y) : x(x), y(y) {}

    T operator[](size_t i) { return raw[i]; }

    Vector2<T> operator + (Vector2<T> v) const { return Vector2<T>(x + v.x, y + v.y); }
    Vector2<T> operator - (Vector2<T> v) const { return Vector2<T>(x - v.x, y - v.y); }

    void operator += (Vector2<T> v) { x += v.x; y += v.y; }
    void operator -= (Vector2<T> v) { x -= v.x; y -= v.y; }

    Vector2<T> scale(T s) const { return Vector2<T>(x * s, y * s); }
    Vector2<T> scale(T sx, T sy) const { return Vector2<T>(x * sx, y * sy); }
    Vector2<T> scale(Vector2<T> s) const { return Vector2<T>(x * s.x, y * s.y); }

    float_t norm() const { return std::sqrt(sqnorm()); }
    T sqnorm() const { return x * x + y * y; }

    Vector2<float> normalized() const { auto n = norm(); return Vector2<float>(x / n, y / n); }

    template<class> friend std::ostream& operator << (std::ostream &s, Vector2<T> v);
    template<class> friend std::istream& operator >> (std::istream &s, Vector2<T> &v);
};

template<class T> struct Vector3
{
    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T iv, iuv, in;};
        T raw[3];
    };

    Vector3() = default;
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    T operator[](size_t i) { return raw[i]; }

    Vector3<T> operator + (const Vector3<T> v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
    Vector3<T> operator - (const Vector3<T> v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }

    void operator += (Vector3<T> v) { x += v.x; y += v.y; z += v.z; }
    void operator -= (Vector3<T> v) { x -= v.x; y -= v.y; z -= v.z; }

    Vector3<T> scale(T s) { return Vector3<T>(x * s, y * s, z * s); }
    Vector3<T> scale(T sx, T sy, T sz) const { return Vector3<T>(x * sx, y * sy, z * sz); }
    Vector3<T> scale(Vector3<T> s) const { return Vector3<T>(x * s.x, y * s.y, z * s.z); }

    float_t norm() const { return std::sqrt(sqnorm()); }
    T sqnorm() const { return x * x + y * y + z * z; }
    Vector3<float> normalized() const { auto n = norm(); return Vector3<float>(x / n, y / n, z / n); }

    template<class> friend inline std::ostream& operator << (std::ostream &s, Vector3<T> v);
    template<class> friend inline std::istream& operator >> (std::istream &s, Vector3<T> &v);
};

typedef Vector2 <int> Vec2Int;
typedef Vector3 <int> Vec3Int;
typedef Vector2 <float> Vec2Float;
typedef Vector3 <float> Vec3Float;
typedef Vector2 <uint16_t> ScreenPoint;

const Vec2Int Vec2IntOne(1, 1);
const Vec3Int Vec3IntOne(1, 1, 1);

const Vec2Float Vec2FloatOne(1.f, 1.f);
const Vec3Float Vec3FloatOne(1.f, 1.f, 1.f);

template<class T> std::ostream& operator << (std::ostream &s, Vector2<T> v)
{
    return s << "(" << v.x << ", " << v.y << ")";
}

template<class T> std::istream& operator >> (std::istream &s, Vector2<T> &v)
{
    return s >> v.x >> v.y;
}

template<class T> std::ostream& operator << (std::ostream &s, Vector3<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<class T> std::istream& operator >> (std::istream &s, Vector3<T> &v)
{
    return s >> v.x >> v.y >> v.z;
}

namespace geometry
{
    template<class T> T dot(Vector2<T> a, Vector2<T> b)
    {
        return a.x * b.x + a.y * b.y;
    }

    template<class T> T dot(Vector3<T> a, Vector3<T> b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    //  x  y  z
    //  ax ay az
    //  bx by bz
    //
    //  a.y * b.z - a.z * b.y = x
    //  a.z * b.x - a.x * b.z = y
    //  a.x * b.y - a.y * b.x = z
    template<class T> Vector3<T> cross(Vector3<T> a, Vector3<T> b)
    {
        return { a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x };
    }

    //  barycentric_screen coordinates are the weights of linear combination of geometry representing a point
    //  inside a polygon.
    //
    //  P = (1 - u - v) * A + u*B + v*C
    //  P = A + u*AB + v*AC
    //  u*AB + v*AC + PA = 0
    //
    //  { u * AB(x) + v * AC(x) + PA(x) = 0;
    //  { u * AB(y) + v * AC(y) + PA(y) = 0;
    //
    //  { (u, v, 1) dot (AB(x), AC(x), PA(x)) = 0;
    //  { (u, v, 1) dot (AB(y), AC(y), PA(y)) = 0;
    //
    //  (u, v, 1) = (AB(x), AC(x), PA(x)) cross (AB(y), AC(y), PA(y))
    //
    inline Vec3Float barycentric_screen(ScreenPoint p, ScreenPoint a, ScreenPoint b, ScreenPoint c)
    {
        Vec3Int uv = cross(Vec3Int(b.x - a.x, c.x - a.x, p.x - a.x),
                           Vec3Int(b.y - a.y, c.y - a.y, p.y - a.y));
        if (uv.z == 0)
            return {-1, 1, 1};

        // normal is actually flipped so z coordinate will be negative
        return { 1.f - (float)(uv.x + uv.y) / -uv.z, (float)uv.x / -uv.z, (float)uv.y / -uv.z };
    }

    // it is a little bit faster (~15%) than calc the exact barycentric_screen coordinates
    inline bool is_in_triangle_screen(ScreenPoint p, ScreenPoint a, ScreenPoint b, ScreenPoint c)
    {
        Vec3Int uv = cross(Vec3Int(b.x - a.x, c.x - a.x, p.x - a.x),
                           Vec3Int(b.y - a.y, c.y - a.y, p.y - a.y));
        uv.z = -uv.z;
        bool isin = (uv.z != 0) &&
                    ((uv.z > 0 && uv.x > 0 && uv.y > 0 && (uv.z - (uv.x + uv.y) > 0)) ||
                     (uv.z < 0 && uv.x < 0 && uv.y < 0 && (uv.z - (uv.x + uv.y) < 0)));

        return isin;
    }
}

namespace calc
{
    inline float clamp0(float val)
    {
        return val < 0 ? 0 : val;
    }

    inline float clamp01(float val)
    {
        return val < 0 ? 0 : val > 1 ? 1 : val;
    }
}

#endif //SIMPLE_RASTERIZER_GEOMETRY_H
