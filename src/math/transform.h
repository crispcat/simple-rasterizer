#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

using Transform  = __Matrix<4, 1, false, float>;
using Projection = __Matrix<4, 4, false, float>;

namespace transform
{
    inline Transform embed(const Vector3<float> &v)
    {
        return {{ v.x, v.y, v.z, 1.f }};
    }

    inline Vector3<float> proj3d(const Transform &tr)
    {
        return { tr.x / tr.t, tr.y / tr.t, tr.z / tr.t };
    }

    inline Projection translate(const Vec3 &delta)
    {
        const float &x = delta.x;
        const float &y = delta.y;
        const float &z = delta.z;
        return {{
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1,
        }};
    }

    inline Projection scale(const Vec3 &scale)
    {
        const float &x = scale.x;
        const float &y = scale.y;
        const float &z = scale.z;
        return {{
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        }};
    }

    inline Projection rotate(const Vec3 &axis, const float &angle)
    {
        const float c = cos(angle);
        const float s = sin(angle);
        const float _1_c = 1 - c;
        const Vec3 &a = axis;
        return {{
            (c + a.x * a.x * _1_c),        (a.x * a.y * _1_c - a.z * s),  (a.x * a.z * _1_c + a.y * s),  0,
            (a.y * a.x * _1_c + a.z * s),  (c + a.y * a.y * _1_c),        (a.y * a.z * _1_c - a.x * s),  0,
            (a.z * a.x * _1_c - a.y * s),  (a.z * a.y * _1_c + a.x * s),  (c + a.z * a.z * _1_c),        0,
             0,                             0,                             0,                            1
        }};
    }

    inline Projection perspective(const float &c_dist)
    {
        const float c = -1.f / c_dist;
        return {{
            1,   0,   0,   0,
            0,  -1,   0,   0,
            0,   0,   1,   0,
            0,   0,   c,   1
        }};
    }
}

#endif //TRANSFORM_H
