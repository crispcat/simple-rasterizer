#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

using Transform = __Matrix<4, 4, false, float>;

inline Transform tr_translate(Vec3 delta)
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

inline Transform tr_scale(Vec3 scale)
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

inline Transform tr_rotate(Vec3 axis, float angle)
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

inline Transform tr_perspective(float c_dist)
{
    float c = -1.f / c_dist;
    return {{
        1,   0,   0,   0,
        0,  -1,   0,   0,
        0,   0,   1,   0,
        0,   0,   c,   1
    }};
}

template <typename T>
T clamp0(T val)
{
    return val < 0 ? 0 : val;
}

template <typename T>
T clamp01(T val)
{
    return val < 0 ? 0 : val > 1 ? 1 : val;
}

template <typename T>
T clamp(T val, T min, T max)
{
    return val < min ? min : val > max ? max : val;
}

#endif //TRANSFORM_H
