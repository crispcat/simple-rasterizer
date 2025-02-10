#include "vector.h"

template<typename T>
T vec_dot(Vector2<T> a, Vector2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename T>
T vec_dot(Vector3<T> a, Vector3<T> b)
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
Vector3<T> vec_cross(Vector3<T> a, Vector3<T> b)
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
//  { (u, v, 1) * (AB(x), AC(x), PA(x)) = 0;
//  { (u, v, 1) * (AB(y), AC(y), PA(y)) = 0;
//
//  (u, v, 1) = (AB(x), AC(x), PA(x)) cross (AB(y), AC(y), PA(y))
inline Vec3 barycentric2(Vec2 p, Vec2 a, Vec2 b, Vec2 c)
{
    Vec3 uv = vec_cross(Vec3(b.x - a.x, c.x - a.x, a.x - p.x),
                        Vec3(b.y - a.y, c.y - a.y, a.y - p.y));
    if (uv.z == 0)
        return { -1.f, 1.f, 1.f };

    return { 1.f - (uv.x + uv.y) / uv.z, uv.x / uv.z, uv.y / uv.z };
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

template<typename T>
std::ostream& operator << (std::ostream &s, Vector4<T> v)
{
    return s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << ")";
}

template<typename T>
std::istream& operator >> (std::istream &s, Vector4<T> &v)
{
    return s >> v.x >> v.y >> v.z >> v.t;
}
