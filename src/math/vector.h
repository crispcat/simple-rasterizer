#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cstdint>
#include <iostream>

template<typename T> struct Vector2;
template<typename T> struct Vector3;
template<typename T> struct Vector4;

using Vec2 = Vector2<float>;
using Vec3 = Vector3<float>;
using Vec4 = Vector4<float>;
using Vec2Int = Vector2<int32_t>;
using Vec3Int = Vector3<int32_t>;
using Vec4Int = Vector4<int32_t>;

namespace vector
{
    template<typename T> T dot(Vector2<T> a, Vector2<T> b);
    template<typename T> T dot(Vector3<T> a, Vector3<T> b);
    template<typename T> Vector3<T> cross(Vector3<T> a, Vector3<T> b);
    inline Vec3 barycentric2(Vec2 p, Vec2 a, Vec2 b, Vec2 c);
}

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

    Vector2<T> operator - () const { return { -x, -y }; }
    Vector2<T> operator + (Vector2<T> v) const { return { x + v.x, y + v.y }; }
    Vector2<T> operator - (Vector2<T> v) const { return { x - v.x, y - v.y }; }
    void operator += (Vector2<T> v) { x += v.x; y += v.y; }
    void operator -= (Vector2<T> v) { x -= v.x; y -= v.y; }
    Vector2<T> operator * (T s) const { return { x * s, y * s }; }
    Vector2<T> operator / (T s) const { return { x / s, y / s }; }

    T sqnorm() const { return x * x + y * y; }
    float_t norm() const { return std::sqrt(sqnorm()); }
    Vector2<float> normalized() const { auto n = norm(); return { x / n, y / n }; }

    Vector2<T> apply(T (*a)(T)) { return { a(x), a(y) }; }

    template<typename V> operator Vector2<V>() { return { static_cast<V>(x), static_cast<V>(y) }; }
    template<typename V> operator Vector3<V>() { return { static_cast<V>(x), static_cast<V>(y), T{} }; }

    template<typename> friend std::ostream& operator << (std::ostream &s, Vector2<T> v);
    template<typename> friend std::istream& operator >> (std::istream &s, Vector2<T> &v);

    static const Vector2<T> ZERO = { 0, 0 };
    static const Vector2<T> ONE  = { 1, 1 };
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

    Vector3<T> operator - () const { return { -x, -y, -z }; }
    Vector3<T> operator + (const Vector3<T> v) const { return { x + v.x, y + v.y, z + v.z }; }
    Vector3<T> operator - (const Vector3<T> v) const { return { x - v.x, y - v.y, z - v.z }; }
    void operator += (Vector3<T> v) { x += v.x; y += v.y; z += v.z; }
    void operator -= (Vector3<T> v) { x -= v.x; y -= v.y; z -= v.z; }
    Vector3<T> operator * (T s) const { return { x * s, y * s, z * s }; }
    Vector3<T> operator / (T s) const { return { x / s, y / s, z / s }; }

    T sqnorm() const { return x * x + y * y + z * z; }
    float_t norm() const { return std::sqrt(sqnorm()); }
    Vector3<float> normalized() const { auto n = norm(); return { x / n, y / n, z / n }; }

    Vector3<T> apply(T (*f)(T)) const { return { f(x), f(y), f(z) }; }

    template<typename V> operator Vector2<V>() const { return { static_cast<V>(x), static_cast<V>(y) }; }
    template<typename V> operator Vector3<V>() const { return { static_cast<V>(x), static_cast<V>(y), static_cast<V>(z) }; }

    template<typename> friend std::ostream& operator << (std::ostream &s, Vector3<T> v);
    template<typename> friend std::istream& operator >> (std::istream &s, Vector3<T> &v);

    static const Vector3<T> ZERO    =  { 0, 0, 0 };
    static const Vector3<T> ONE     =  { 1, 1, 1 };
    static const Vector3<T> RIGHT   =  { 1, 0, 0 };
    static const Vector3<T> UP      =  { 0, 1, 0 };
    static const Vector3<T> FORWARD =  { 0, 0, 1 };
    static const Vector3<T> LEFT    =  { -1,  0,  0 };
    static const Vector3<T> DOWN    =  {  0, -1,  0 };
    static const Vector3<T> BACK    =  {  0,  0, -1 };
};

template<typename T>
struct Vector4
{
    union
    {
        struct { T x, y, z, t; };
        T raw[4];
    };

    Vector4() : x(), y(), z(), t() { }
    Vector4(T x, T y, T z, T t) : x(x), y(y), z(z), t(t) { }
    explicit Vector4(T raw[4]) : raw(raw) { }

    T operator[](size_t i) { return raw[i]; }

    Vector4<T> operator - () const { return { -x, -y, -z, -t }; }
    Vector4<T> operator + (const Vector4<T> v) const { return { x + v.x, y + v.y, z + v.z, t + v.t }; }
    Vector4<T> operator - (const Vector4<T> v) const { return { x - v.x, y - v.y, z - v.z, t - v.t }; }
    void operator += (Vector4<T> v) { x += v.x; y += v.y; z += v.z; t += v.t; }
    void operator -= (Vector4<T> v) { x -= v.x; y -= v.y; z -= v.z; t -= v.t; }
    Vector4 operator * (T s) const { return { x * s, y * s, z * s, t * s }; }
    Vector4 operator / (T s) const { return { x / s, y / s, z / s, t / s }; }

    T sqnorm() const { return x * x + y * y + z * z + t * t; }
    float_t norm() const { return std::sqrt(sqnorm()); }
    Vector4<float> normalized() const { auto n = norm(); return { x / n, y / n, z / n, t / n }; }

    Vector4<T> apply(T (*f)(T)) const { return { f(x), f(y), f(z), f(t) }; }

    template<typename V>
    operator Vector2<V>() const { return { static_cast<V>(x), static_cast<V>(y) }; }
    template<typename V>
    operator Vector3<V>() const { return { static_cast<V>(x), static_cast<V>(y), static_cast<V>(z) }; }
    template<typename V>
    operator Vector4<V>() const { return { static_cast<V>(x), static_cast<V>(y), static_cast<V>(z), static_cast<V>(t) }; }

    template<typename>
    friend std::ostream& operator << (std::ostream &s, Vector3<T> v);
    template<typename>
    friend std::istream& operator >> (std::istream &s, Vector3<T> &v);
};

template<typename T>
std::ostream& operator << (std::ostream &s, const Vector2<T> v)
{
    return s << "(" << v.x << ", " << v.y << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector2<T> &v)
{
    return s >> v.x >> v.y;
}

template<typename T>
std::ostream& operator << (std::ostream &s, const Vector3<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector3<T> &v)
{
    return s >> v.x >> v.y >> v.z;
}

template<typename T>
std::ostream& operator << (std::ostream &s, const Vector4<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector4<T> &v)
{
    return s >> v.x >> v.y >> v.z >> v.t;
}

#endif //VECTOR_H
