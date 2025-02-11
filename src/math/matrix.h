#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>
#include "vector.h"

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
struct __Matrix
{
    static const std::size_t SIZE = ROWS * COLS;

    union
    {
        T _el[SIZE];
        struct { T x, y, z, t; };
    };

    T get(size_t row, size_t col);
    void set(size_t row, size_t col, T val);

    __Matrix() : _el { 0 } { }

    __Matrix(const T (&arr)[SIZE]) { std::copy(arr, arr + SIZE, _el); }

    __Matrix(const T (&&arr)[SIZE]) { std::move(arr, arr + SIZE, _el); }

    static __Matrix embed(Vector3<T> v);

    Vector3<T> proj();

    __Matrix<COLS, ROWS, !TRANSPOSED, T> transpose();

    void operator += (const __Matrix &m);
    void operator -= (const __Matrix &m);
    __Matrix<ROWS, COLS, false, T> operator + (const __Matrix &m);
    __Matrix<ROWS, COLS, false, T> operator - (const __Matrix &m);

    template <size_t R_COLS, bool R_TRANSPOSED>
    __Matrix<ROWS, R_COLS, false, T> operator * (const __Matrix<COLS, R_COLS, R_TRANSPOSED, T> &m);

    friend std::ostream& operator << (std::ostream &s, __Matrix m);
};

using Hom = __Matrix<4, 1, false, float>;

template <std::size_t ROWS, std::size_t COLS, typename T>
using Matrix = __Matrix<ROWS, COLS, false, T>;

template <std::size_t ROWS, std::size_t COLS, typename T>
using MatrixTr = __Matrix<ROWS, COLS, true, T>;

#endif //MATRIX_H
