#ifndef SIMPLE_RASTERIZER_TESTS_H
#define SIMPLE_RASTERIZER_TESTS_H

#include "renderer.h"

inline void draw_primitives_triangles(RenderContext &c)
{
    c.start_frame();
    c.foregr_color = color32::WHITE;
    c.triangle_lined(10,70, 50,160, 70,80);
    c.triangle_lined(180,50, 150,1, 70,180);
    c.triangle_lined(180,150, 120,160, 130,180);
#ifndef PROFILE
    c.foregr_color = color32::GREEN;
    c.line(10,70,  50,160);
    c.line(50,160, 70,80);
    c.line(10,70,  70,80);
    c.line(180,50, 150,1);
    c.line(180,50, 70,180);
    c.line(150,1,  70,180);
    c.line(180,150, 120,160);
    c.line(180,150, 130,180);
    c.line(120,160, 130,180);
#endif
}

inline void draw_primitives_lines(RenderContext &c)
{
    c.start_frame();
    c.foregr_color = color32::GREEN;
    c.line(50, 50, 75, 75);
    c.line(50, 50, 75, 25);
    c.line(50, 50, 25, 25);
    c.line(50, 50, 25, 75);

    c.foregr_color = color32::RED;
    c.line(50, 50, 75, 85);
    c.line(50, 50, 75, 65);
    c.line(50, 50, 75, 35);
    c.line(50, 50, 75, 15);
    c.line(50, 50, 25, 15);
    c.line(50, 50, 25, 35);
    c.line(50, 50, 25, 65);
    c.line(50, 50, 25, 85);

    c.foregr_color = color32::WHITE;
    c.line(50, 50, 75, 50);
    c.line(50, 50, 50, 25);
    c.line(50, 50, 25, 50);
    c.line(50, 50, 50, 75);
}

inline void calc_vectors()
{
//#ifdef PROFILE
//    std::cout.setstate(std::ios_base::failbit);
//    for (uint32_t i = 0; i < 100000; i++) {
//#endif
    Vec2Int vec2intA;
    Vec2Int vec2intB(-4, 3);
    std::cout << "Vec2Int:\n";
    std::cout << vec2intA << " + " << vec2intB << " = " << vec2intA + vec2intB << '\n';
    std::cout << vec2intA << " - " << vec2intB << " = " << vec2intA - vec2intB << '\n';
    std::cout << vec2intA << " dot " << vec2intB << " = " << vector::dot(vec2intA, vec2intB) << '\n';
    std::cout << "norm" << vec2intA << " = " << vec2intA.norm() << '\n';
    std::cout << "sqnorm" << vec2intA << " = " << vec2intA.sqnorm() << '\n';
    std::cout << "normalized" << vec2intA << " = " << vec2intA.normalized() << '\n';

    Vec2 vec2floatA(1.744, -2.33);
    Vec2 vec2floatB(-4.0, 3.5);
    std::cout << "Vec2:\n";
    std::cout << vec2floatA << " + " << vec2floatB << " = " << vec2floatA + vec2floatB << '\n';
    std::cout << vec2floatA << " - " << vec2floatB << " = " << vec2floatA - vec2floatB << '\n';
    std::cout << vec2floatA << " dot " << vec2floatB << " = " << vector::dot(vec2floatA, vec2floatB) << '\n';
    std::cout << "norm" << vec2floatA << " = " << vec2floatA.norm() << '\n';
    std::cout << "sqnorm" << vec2floatA << " = " << vec2floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec2floatA << " = " << vec2floatA.normalized() << '\n';

    Vec3Int vec3intA(1, -2, 8);
    Vec3Int vec3intB(-4, 3, -1);
    std::cout << "Vec3Int:\n";
    std::cout << vec3intA << " + " << vec3intB << " = " << vec3intA + vec3intB << '\n';
    std::cout << vec3intA << " - " << vec3intB << " = " << vec3intA - vec3intB << '\n';
    std::cout << vec3intA << " dot " << vec3intB << " = " << vector::dot(vec3intA, vec3intB) << '\n';
    std::cout << vec3intA << " cross " << vec3intB << " = " << vector::cross(vec3intA, vec3intB) << '\n';
    std::cout << "norm" << vec3intA << " = " << vec3intA.norm() << '\n';
    std::cout << "sqnorm" << vec3intA << " = " << vec3intA.sqnorm() << '\n';
    std::cout << "normalized" << vec3intA << " = " << vec3intA.normalized() << '\n';

    Vec3 vec3floatA(1.744, -2.33, 8.3333333);
    Vec3 vec3floatB(-4.0, 3.5, -1.91);
    std::cout << "Vec3:\n";
    std::cout << vec3floatA << " + " << vec3floatB << " = " << vec3floatA + vec3floatB << '\n';
    std::cout << vec3floatA << " - " << vec3floatB << " = " << vec3floatA - vec3floatB << '\n';
    std::cout << vec3floatA << " dot " << vec3floatB << " = " << vector::dot(vec3floatA, vec3floatB) << '\n';
    std::cout << vec3floatA << " cross " << vec3floatB << " = " << vector::cross(vec3floatA, vec3floatB) << '\n';
    std::cout << "norm" << vec3floatA << " = " << vec3floatA.norm() << '\n';
    std::cout << "sqnorm" << vec3floatA << " = " << vec3floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec3floatA << " = " << vec3floatA.normalized() << '\n';
    std::cout << '\n';
//#ifdef PROFILE
//    }
//#endif
}

inline void calc_matrices()
{
    Matrix<2, 5, float> m1 {{
        3,    7,  -2,   12,  4,
        3.4f, 1.f, 0.f, 5.f, 33
    }};

    Matrix<5, 2, float> m2 {{
        3, 3,
        0, 0,
        5, 5,
        4, 4,
        2, 1
    }};

    std::cout << "m1:" << '\n' << m1 << '\n' << '\n';
    std::cout << "m2:" << '\n' << m2 << '\n' << '\n';

    auto m3 = m1 * m2;
    std::cout << "m3:" << '\n' << m3 << '\n' << '\n';

    float m4d[10] = {
        3,    7,  -2,   12,  4,
        3.4f, 1.f, 0.f, 5.f, 33
    };

    Matrix<5, 2, float>m4 { m4d };
    std::cout << "m4:" << '\n' << m4 << '\n' << '\n';

    Matrix<5, 2, float>m5 {{
        3,    7,  -2,   12,  4,
        3.4f, 1.f, 0.f, 5.f, 33,
    }};

    std::cout << "m5:" << '\n' << m5 << '\n';

    Matrix<5, 2, float>m6 {{ 3, 7, -2, 12, 4 }};
    std::cout << "m6:" << '\n' << m6 << '\n';
}

#endif //SIMPLE_RASTERIZER_TESTS_H
