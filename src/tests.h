#ifndef SIMPLE_RASTERIZER_TESTS_H
#define SIMPLE_RASTERIZER_TESTS_H

#include "geometry.h"
#include "context.h"

void draw_primitives_triangles(Context &c)
{
    c.color = COLOR_WHITE;
    c.triangle_lined(10,70, 50,160, 70,80);
    c.triangle_lined(180,50, 150,1, 70,180);
    c.triangle_lined(180,150, 120,160, 130,180);
#ifndef PROFILE
    c.color = COLOR_GREEN;
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

void draw_primitives_lines(Context &c)
{
    c.color = COLOR_GREEN;
    c.line(50, 50, 75, 75);
    c.line(50, 50, 75, 25);
    c.line(50, 50, 25, 25);
    c.line(50, 50, 25, 75);

    c.color = COLOR_RED;
    c.line(50, 50, 75, 85);
    c.line(50, 50, 75, 65);
    c.line(50, 50, 75, 35);
    c.line(50, 50, 75, 15);
    c.line(50, 50, 25, 15);
    c.line(50, 50, 25, 35);
    c.line(50, 50, 25, 65);
    c.line(50, 50, 25, 85);

    c.color = COLOR_WHITE;
    c.line(50, 50, 75, 50);
    c.line(50, 50, 50, 25);
    c.line(50, 50, 25, 50);
    c.line(50, 50, 50, 75);
}

void defileVectors()
{
#ifdef PROFILE
    std::cout.setstate(std::ios_base::failbit);
    for (uint32_t i = 0; i < 100000; i++) {
#endif
    Vec2Int vec2intA(1, -2);
    Vec2Int vec2intB(-4, 3);
    std::cout << "Vec2Int:\n";
    std::cout << vec2intA << " + " << vec2intB << " = " << vec2intA + vec2intB << '\n';
    std::cout << vec2intA << " - " << vec2intB << " = " << vec2intA - vec2intB << '\n';
    std::cout << vec2intA << " dot " << vec2intB << " = " << geometry::dot(vec2intA, vec2intB) << '\n';
    std::cout << "norm" << vec2intA << " = " << vec2intA.norm() << '\n';
    std::cout << "sqnorm" << vec2intA << " = " << vec2intA.sqnorm() << '\n';
    std::cout << "normalized" << vec2intA << " = " << vec2intA.normalized() << '\n';
    std::cout << "scale -5" << vec2intA << " = " << vec2intA.scale(-5) << '\n';

    Vec2Float vec2floatA(1.744, -2.33);
    Vec2Float vec2floatB(-4.0, 3.5);
    std::cout << "Vec2Float:\n";
    std::cout << vec2floatA << " + " << vec2floatB << " = " << vec2floatA + vec2floatB << '\n';
    std::cout << vec2floatA << " - " << vec2floatB << " = " << vec2floatA - vec2floatB << '\n';
    std::cout << vec2floatA << " dot " << vec2floatB << " = " << geometry::dot(vec2floatA, vec2floatB) << '\n';
    std::cout << "norm" << vec2floatA << " = " << vec2floatA.norm() << '\n';
    std::cout << "sqnorm" << vec2floatA << " = " << vec2floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec2floatA << " = " << vec2floatA.normalized() << '\n';
    std::cout << "scale -5" << vec2floatA << " = " << vec2floatA.scale(-5) << '\n';

    Vec3Int vec3intA(1, -2, 8);
    Vec3Int vec3intB(-4, 3, -1);
    std::cout << "Vec3Int:\n";
    std::cout << vec3intA << " + " << vec3intB << " = " << vec3intA + vec3intB << '\n';
    std::cout << vec3intA << " - " << vec3intB << " = " << vec3intA - vec3intB << '\n';
    std::cout << vec3intA << " dot " << vec3intB << " = " << geometry::dot(vec3intA, vec3intB) << '\n';
    std::cout << vec3intA << " cross " << vec3intB << " = " << geometry::cross(vec3intA, vec3intB) << '\n';
    std::cout << "norm" << vec3intA << " = " << vec3intA.norm() << '\n';
    std::cout << "sqnorm" << vec3intA << " = " << vec3intA.sqnorm() << '\n';
    std::cout << "normalized" << vec3intA << " = " << vec3intA.normalized() << '\n';
    std::cout << "scale -5" << vec3intA << " = " << vec3intA.scale(-5) << '\n';

    Vec3Float vec3floatA(1.744, -2.33, 8.3333333);
    Vec3Float vec3floatB(-4.0, 3.5, -1.91);
    std::cout << "Vec3Float:\n";
    std::cout << vec3floatA << " + " << vec3floatB << " = " << vec3floatA + vec3floatB << '\n';
    std::cout << vec3floatA << " - " << vec3floatB << " = " << vec3floatA - vec3floatB << '\n';
    std::cout << vec3floatA << " dot " << vec3floatB << " = " << geometry::dot(vec3floatA, vec3floatB) << '\n';
    std::cout << vec3floatA << " cross " << vec3floatB << " = " << geometry::cross(vec3floatA, vec3floatB) << '\n';
    std::cout << "norm" << vec3floatA << " = " << vec3floatA.norm() << '\n';
    std::cout << "sqnorm" << vec3floatA << " = " << vec3floatA.sqnorm() << '\n';
    std::cout << "normalized" << vec3floatA << " = " << vec3floatA.normalized() << '\n';
    std::cout << "scale -5" << vec3floatA << " = " << vec3floatA.scale(-5) << '\n';
#ifdef PROFILE
    }
#endif
}

#endif //SIMPLE_RASTERIZER_TESTS_H
