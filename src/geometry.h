#ifndef SIMPLE_RASTERIZER_GEOMETRY_H
#define SIMPLE_RASTERIZER_GEOMETRY_H

#include <cmath>
#include <iostream>

template<class T> struct Vector2;
template<class T> struct Vector3;
template<size_t N, size_t M, class T> struct Matrix;

template<class T>
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

    template<class V>
    operator Vector2<V>() { return Vector2<V>(static_cast<V>(x), static_cast<V>(y)); }
    template<class V>
    operator Vector3<V>() { return Vector3<V>(static_cast<V>(x), static_cast<V>(y), T{}); }

    template<class>
    friend std::ostream& operator << (std::ostream &s, Vector2<T> v);
    template<class>
    friend std::istream& operator >> (std::istream &s, Vector2<T> &v);
};

template<class T>
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

    Vector3<T> scale(T s) { return Vector3<T>(x * s, y * s, z * s); }
    Vector3<T> scale(T sx, T sy, T sz) const { return Vector3<T>(x * sx, y * sy, z * sz); }
    Vector3<T> scale(Vector3<T> s) const { return Vector3<T>(x * s.x, y * s.y, z * s.z); }

    float_t norm() const { return std::sqrt(sqnorm()); }
    T sqnorm() const { return x * x + y * y + z * z; }
    Vector3<float> normalized() const { auto n = norm(); return Vector3<float>(x / n, y / n, z / n); }

    operator Vector2<T>() const { return Vector2<T>(x, y); }
    Vector3<T> apply(T (*a)(T)) const { return Vector3<T>(a(x), a(y), a(z)); }

    template<class V>
    operator Vector3<V>() const { return Vector3<V>(static_cast<V>(x), static_cast<V>(y), static_cast<V>(z)); }
    template<class V>
    operator Vector2<V>() const { return Vector2<V>(static_cast<V>(x), static_cast<V>(y)); }

    template<class>
    friend inline std::ostream& operator << (std::ostream &s, Vector3<T> v);
    template<class>
    friend inline std::istream& operator >> (std::istream &s, Vector3<T> &v);
};

using Vec2 = Vector2<float>;
using Vec3 = Vector3<float>;
using Vec2Int = Vector2<int>;
using Vec3Int = Vector3<int>;
using ScreenPoint = Vector2<uint16_t>;

const Vec2 Vec2One(1.f, 1.f);
const Vec3 Vec3One(1.f, 1.f, 1.f);
const Vec2Int Vec2IntOne(1, 1);
const Vec3Int Vec3IntOne(1, 1, 1);

namespace geometry
{
    template<class T>
    T dot(Vector2<T> a, Vector2<T> b)
    {
        return a.x * b.x + a.y * b.y;
    }

    template<class T>
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
    template<class T>
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
    //
    inline Vec3 barycentric_screen(ScreenPoint p, ScreenPoint a, ScreenPoint b, ScreenPoint c)
    {
        Vec3Int uv = cross(Vec3Int(b.x - a.x, c.x - a.x, p.x - a.x),
                           Vec3Int(b.y - a.y, c.y - a.y, p.y - a.y));
        if (uv.z == 0)
            return {-1, 1, 1};

        // norm is actually flipped, so z coordinate will be negative
        return { 1.f - (float)(uv.x + uv.y) / -uv.z, (float)uv.x / -uv.z, (float)uv.y / -uv.z };
    }
}

template </*ROWS*/size_t N,/*COLS*/size_t M, class T>
struct Matrix
{
    static const size_t SIZE = N * M;

    T el[SIZE];

    Matrix() : el { 0 } { }

    explicit Matrix(T arr[SIZE]) : el { 0 }
    {
        std::copy(arr, arr + SIZE, el);
    }

    Matrix(std::initializer_list<T> il) : el { 0 }
    {
        std::move(il.begin(), il.end(), el);
    }

    T* operator [] (size_t i)
    {
        return el + i * M;
    }

    void operator += (Matrix<N, M, T> m)
    {
        for (size_t i = 0; i < SIZE; i++)
            el[i] += m.el[i];
    }

    void operator -= (Matrix<N, M, T> m)
    {
        for (size_t i = 0; i < SIZE; i++)
            el[i] -= m.el[i];
    }

    Matrix<N, M, T> operator + (Matrix<N, M, T> m) const
    {
        Matrix<N, M, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] + m.el[i];
        return res;
    }

    Matrix<N, M, T> operator - (Matrix<N, M, T> m) const
    {
        Matrix<N, M, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] - m.el[i];
        return res;
    }

    template<size_t L>
    Matrix<N, L, T> operator * (Matrix<M, L, T> m) const
    {
        Matrix<N, L, T> res;
        for (size_t i = 0; i < N * L; i++)
            for (size_t j = 0; j < M; j++)
                res.el[i] += el[(i / L) * M + j] * m.el[j * L + (i % L)];
        return res;
    }

    static Matrix<N, M, T> identity()
    {
        static_assert(N == M, "identity matrix construction require a square matrix type");
        Matrix<N, M, T> id;
        for (size_t i = 0; i < SIZE; i += N + 1)
            id.el[i] = 1;
        return id;
    }

    template<class>
    friend inline std::ostream& operator << (std::ostream &s, Matrix<N, M, T> m);
};

inline void transform(Vec3 &vec, Matrix<4, 4, float> transformer)
{
    Matrix<4, 1, float> hom { vec.x, vec.y, vec.z, 1.f };
    hom = transformer * hom;
    vec.x = hom[0][0] / hom[3][0];
    vec.y = hom[1][0] / hom[3][0];
    vec.z = hom[2][0] / hom[3][0];
}

namespace transformer
{
    inline Matrix<4, 4, float> perspective(float c_dist)
    {
        auto id = Matrix<4, 4, float>::identity();
        id[3][2] = -1.f / c_dist;
        return id;
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

template<class T>
std::ostream& operator << (std::ostream &s, Vector2<T> v)
{
    return s << "(" << v.x << ", " << v.y << ")";
}

template<class T>
std::istream& operator >> (std::istream &s, Vector2<T> &v)
{
    return s >> v.x >> v.y;
}

template<class T>
std::ostream& operator << (std::ostream &s, Vector3<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<class T>
std::istream& operator >> (std::istream &s, Vector3<T> &v)
{
    return s >> v.x >> v.y >> v.z;
}

template<size_t N, size_t M, class T>
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
