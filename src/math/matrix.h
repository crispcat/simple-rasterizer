#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>
#include "vector.h"

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
struct __Matrix;

template <std::size_t ROWS, std::size_t COLS, typename T>
using Matrix = __Matrix<ROWS, COLS, false, T>;

template <std::size_t ROWS, std::size_t COLS, typename T>
using MatrixTr = __Matrix<ROWS, COLS, true, T>;

namespace matrix
{
    template <std::size_t N, typename T>
    __Matrix<N, N, false, T> identity();
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
struct __Matrix
{
    static const std::size_t SIZE = ROWS * COLS;

    union
    {
        T _el[SIZE];
        struct { T x, y, z, t; };
    };

    T get(std::size_t row, std::size_t col);
    void set(std::size_t row, std::size_t col, T val);

    __Matrix() : _el { 0 } { }

    explicit __Matrix(const T (&arr)[SIZE]) { std::copy(arr, arr + SIZE, _el); }

    explicit __Matrix(const T (&&arr)[SIZE]) { std::move(arr, arr + SIZE, _el); }

    __Matrix<COLS, ROWS, !TRANSPOSED, T> transpose();

    void operator += (const __Matrix &m);
    void operator -= (const __Matrix &m);
    __Matrix<ROWS, COLS, false, T> operator + (const __Matrix &m) const;
    __Matrix<ROWS, COLS, false, T> operator - (const __Matrix &m) const;

    template <size_t R_COLS, bool R_TRANSPOSED>
    __Matrix<ROWS, R_COLS, false, T> operator * (const __Matrix<COLS, R_COLS, R_TRANSPOSED, T> &m) const;

    friend std::ostream& operator << (std::ostream &s, __Matrix m);

    template <typename V> explicit operator Vector2<V>() const;
    template <typename V> explicit operator Vector3<V>() const;
    template <typename V> explicit operator Vector4<V>() const;
};

#endif //MATRIX_H
