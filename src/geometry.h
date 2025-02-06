#ifndef SIMPLE_RASTERIZER_GEOMETRY_H
#define SIMPLE_RASTERIZER_GEOMETRY_H

#include <cmath>
#include <cstdint>
#include <iostream>

template<typename T> struct Vector2;
template<typename T> struct Vector3;
template<size_t N, size_t M, class T> struct Matrix;

template<typename T>
struct Vector2
{
    union
    {
        struct{ T x, y; };
        struct{ T u, v; };
        T raw[2];
    };

    Vector2() : x(), y() { }
    Vector2(T x, T y) : x(x), y(y) { }
    explicit Vector2(T raw[2]) : raw(raw) { }

    T operator[](size_t i) { return raw[i]; }

    Vector2<T> operator + (Vector2<T> v) const { return Vector2<T>(x + v.x, y + v.y); }
    Vector2<T> operator - (Vector2<T> v) const { return Vector2<T>(x - v.x, y - v.y); }

    void operator += (Vector2<T> v) { x += v.x; y += v.y; }
    void operator -= (Vector2<T> v) { x -= v.x; y -= v.y; }

    Vector2<T> operator - () const { return Vector2<T>(-x, -y); }

    Vector2<T> scale(T s) const { return Vector2<T>(x * s, y * s); }
    Vector2<T> scale(T sx, T sy) const { return Vector2<T>(x * sx, y * sy); }
    Vector2<T> scale(Vector2<T> s) const { return Vector2<T>(x * s.x, y * s.y); }

    float_t norm() const { return std::sqrt(sqnorm()); }
    T sqnorm() const { return x * x + y * y; }
    Vector2<float> normalized() const { auto n = norm(); return Vector2<float>(x / n, y / n); }

    operator Vector3<T>() { return Vector3<T>(x, y); }
    Vector2<T> apply(T (*a)(T)) { return Vector2<T>(a(x), a(y)); }

    template<typename V>
    operator Vector2<V>() { return Vector2<V>(static_cast<V>(x), static_cast<V>(y)); }
    template<typename V>
    operator Vector3<V>() { return Vector3<V>(static_cast<V>(x), static_cast<V>(y), T{}); }

    template<typename>
    friend std::ostream& operator << (std::ostream &s, Vector2<T> v);
    template<typename>
    friend std::istream& operator >> (std::istream &s, Vector2<T> &v);

    static constexpr Vector2<T> zero () { return { }; }
    static constexpr Vector2<T> one  () { return { 1, 1 }; }
};

template<typename T>
struct Vector3
{
    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T iv, iuv, in;};
        T raw[3];
    };

    Vector3() : x(), y(), z() { }
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { }
    explicit Vector3(T raw[3]) : raw(raw) { }

    T operator[](size_t i) { return raw[i]; }

    Vector3<T> operator - () const { return Vector3<T>(-x, -y, -z); }
    Vector3<T> operator + (const Vector3<T> v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
    Vector3<T> operator - (const Vector3<T> v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }

    void operator += (Vector3<T> v) { x += v.x; y += v.y; z += v.z; }
    void operator -= (Vector3<T> v) { x -= v.x; y -= v.y; z -= v.z; }

    Vector3<T> scale(Vector3<T> s) const { return Vector3<T>(x * s.x, y * s.y, z * s.z); }
    Vector3<T> scale(T s) { return Vector3<T>(x * s, y * s, z * s); }
    Vector3<T> scale(T sx, T sy, T sz) const { return Vector3<T>(x * sx, y * sy, z * sz); }

    float_t norm() const { return std::sqrt(sqnorm()); }
    T sqnorm() const { return x * x + y * y + z * z; }
    Vector3<float> normalized() const { auto n = norm(); return Vector3<float>(x / n, y / n, z / n); }

    operator Vector2<T>() const { return Vector2<T>(x, y); }
    Vector3<T> apply(T (*f)(T)) const { return Vector3<T>(f(x), f(y), f(z)); }
    Vector3<T> apply(T (*fx)(T), T (*fy)(T)) const { return Vector3<T>(fx(x), fy(y), z); }
    Vector3<T> apply(T (*fx)(T), T (*fy)(T), T (*fz)(T)) const { return Vector3<T>(fx(x), fy(y), fz(z)); }

    template<typename V>
    operator Vector3<V>() const { return Vector3<V>(static_cast<V>(x), static_cast<V>(y), static_cast<V>(z)); }
    template<typename V>
    operator Vector2<V>() const { return Vector2<V>(static_cast<V>(x), static_cast<V>(y)); }

    template<typename>
    friend inline std::ostream& operator << (std::ostream &s, Vector3<T> v);
    template<typename>
    friend inline std::istream& operator >> (std::istream &s, Vector3<T> &v);

    static constexpr Vector3<T> zero    () { return { }; }
    static constexpr Vector3<T> one     () { return { 1, 1, 1 }; }
    static constexpr Vector3<T> right   () { return { 1, 0, 0 }; }
    static constexpr Vector3<T> up      () { return { 0, 1, 0 }; }
    static constexpr Vector3<T> forward () { return { 0, 0, 1 }; }
    static constexpr Vector3<T> left    () { return { -1,  0,  0 }; }
    static constexpr Vector3<T> down    () { return {  0, -1,  0 }; }
    static constexpr Vector3<T> back    () { return {  0,  0, -1 }; }
};

using Vec2 = Vector2<float>;
using Vec3 = Vector3<float>;
using Vec2Int = Vector2<int32_t>;
using Vec3Int = Vector3<int32_t>;
using ScreenPoint = Vector2<uint16_t>;

template<typename T>
T dot(Vector2<T> a, Vector2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename T>
T dot(Vector3<T> a, Vector3<T> b)
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
template<typename T>
Vector3<T> cross(Vector3<T> a, Vector3<T> b)
{
    return { a.y * b.z - a.z * b.y,
             a.z * b.x - a.x * b.z,
             a.x * b.y - a.y * b.x };
}

//  barycentric coordinates is linear combination of the face vectors with weights
//  representing a point inside a polygon.
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
//  TODO: make template
inline Vec3 barycentric(ScreenPoint p, ScreenPoint a, ScreenPoint b, ScreenPoint c)
{
    Vec3Int uv = cross(Vec3Int(b.x - a.x, c.x - a.x, a.x - p.x),
                       Vec3Int(b.y - a.y, c.y - a.y, a.y - p.y));
    if (uv.z == 0)
        return { -1, 1, 1 };
    return { 1.f - (float)(uv.x + uv.y) / uv.z, (float)uv.x / uv.z, (float)uv.y / uv.z };
}

template <size_t ROWS, size_t COLS, typename T>
struct Matrix
{
    static const size_t SIZE = ROWS * COLS;

    union
    {
        T el[SIZE];
        struct { T x, y, z, t; };
    };

    Matrix() : el { 0 } { }

    Matrix(const T (&arr)[SIZE])
    {
        std::copy(arr, arr + SIZE, el);
    }

    explicit Matrix(Vector3<T> vec) : Matrix({ vec.x, vec.y, vec.z, 1.f }) { }

    Vector3<T> proj3d()
    {
        static_assert(ROWS == 4 && COLS == 1, "only homogeneous matrix 4 x 1 can be projected back in 3d");
        return { x / t, y / t, z / t };
    }

    T* operator [] (size_t i)
    {
        return el + i * COLS;
    }

    void operator += (Matrix<ROWS, COLS, T> m)
    {
        for (size_t i = 0; i < SIZE; i++)
            el[i] += m.el[i];
    }

    void operator -= (Matrix<ROWS, COLS, T> m)
    {
        for (size_t i = 0; i < SIZE; i++)
            el[i] -= m.el[i];
    }

    Matrix<ROWS, COLS, T> operator + (Matrix<ROWS, COLS, T> m) const
    {
        Matrix<ROWS, COLS, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] + m.el[i];
        return res;
    }

    Matrix<ROWS, COLS, T> operator - (Matrix<ROWS, COLS, T> m) const
    {
        Matrix<ROWS, COLS, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] - m.el[i];
        return res;
    }

    template<size_t L>
    Matrix<ROWS, L, T> operator * (Matrix<COLS, L, T> m) const
    {
        Matrix<ROWS, L, T> res;
        for (size_t i = 0; i < ROWS * L; i++)
            for (size_t j = 0; j < COLS; j++)
                res.el[i] += el[(i / L) * COLS + j] * m.el[j * L + (i % L)];
        return res;
    }

    static constexpr Matrix<ROWS, COLS, T> identity()
    {
        static_assert(ROWS == COLS, "identity matrix construction require a square matrix type");
        Matrix<ROWS, COLS, T> id;
        for (size_t i = 0; i < SIZE; i += ROWS + 1)
            id.el[i] = 1;
        return id;
    }

    template<typename>
    friend inline std::ostream& operator << (std::ostream &s, Matrix<ROWS, COLS, T> m);
};

using Hom = Matrix<4, 1, float>;
using Transform = Matrix<4, 4, float>;

inline Transform translate(Vec3 delta)
{
    float &x = delta.x;
    float &y = delta.y;
    float &z = delta.z;
    return {{
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    }};
}

inline Transform scale(Vec3 scale)
{
    float &x = scale.x;
    float &y = scale.y;
    float &z = scale.z;
    return {{
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    }};
}

inline Transform rotate(Vec3 axis, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    float _1_c = 1 - c;
    Vec3 &a = axis;
    return {{
        (c + a.x * a.x * _1_c),        (a.x * a.y * _1_c - a.z * s),  (a.x * a.z * _1_c + a.y * s),  0,
        (a.y * a.x * _1_c + a.z * s),  (c + a.y * a.y * _1_c),        (a.y * a.z * _1_c - a.x * s),  0,
        (a.z * a.x * _1_c - a.y * s),  (a.z * a.y * _1_c + a.x * s),  (c + a.z * a.z * _1_c),        0,
         0,                             0,                             0,                            1
    }};
}

inline Transform perspective(float c_dist)
{
    float c = -1.f / c_dist;
    return {{
        1,   0,   0,   0,
        0,  -1,   0,   0,
        0,   0,   1,   0,
        0,   0,   c,   1
    }};
}

template <class T>
inline T clamp0(T val)
{
    return val < 0 ? 0 : val;
}

template <class T>
inline T clamp01(T val)
{
    return val < 0 ? 0 : val > 1 ? 1 : val;
}

template <class T>
inline T clamp(T val, T min, T max)
{
    return val < min ? min : val > max ? max : val;
}

template<typename T>
std::ostream& operator << (std::ostream &s, Vector2<T> v)
{
    return s << "(" << v.x << ", " << v.y << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector2<T> &v)
{
    return s >> v.x >> v.y;
}

template<typename T>
std::ostream& operator << (std::ostream &s, Vector3<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector3<T> &v)
{
    return s >> v.x >> v.y >> v.z;
}

template<size_t N, size_t M, typename T>
std::ostream& operator << (std::ostream &s, Matrix<N, M, T> m)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < M; j++)
            s << m[i][j] << ' ';
        s << '\n';
    }
    return s;
}

#endif //SIMPLE_RASTERIZER_GEOMETRY_H
