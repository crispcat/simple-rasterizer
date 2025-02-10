#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <cstdint>
#include <iostream>

#include "vector.h"

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
struct Matrix
{
    static const std::size_t SIZE = ROWS * COLS;

    union
    {
        T _el[SIZE];
        struct { T x, y, z, t; };
    };

    inline T get(std::size_t row, std::size_t col);
    inline void set(std::size_t row, std::size_t col, T val);

    Matrix() : _el { 0 } { }

    Matrix(const T (&arr)[SIZE]) { std::copy(arr, arr + SIZE, _el); }

    Matrix(const T (&&arr)[SIZE]) { std::move(arr, arr + SIZE, _el); }

    static Matrix embed(Vector3<T> v);

    Vector3<T> proj();

    template<std::size_t RROWS, std::size_t RCOLS, bool RTRANSPOSED>
    void operator += (Matrix<RROWS, RCOLS, RTRANSPOSED, T> m);

    template<std::size_t RROWS, std::size_t RCOLS, bool RTRANSPOSED>
    void operator -= (Matrix<RROWS, RCOLS, RTRANSPOSED, T> m);

    template<std::size_t RROWS, std::size_t RCOLS, bool RTRANSPOSED>
    Matrix operator + (Matrix<RROWS, RCOLS, RTRANSPOSED, T> m) const;

    template<std::size_t RROWS, std::size_t RCOLS, bool RTRANSPOSED>
    Matrix operator - (Matrix<RROWS, RCOLS, RTRANSPOSED, T> m) const;

    template<std::size_t RROWS, std::size_t RCOLS, bool RTRANSPOSED>
    Matrix operator * (Matrix<RROWS, RCOLS, RTRANSPOSED, T> m) const;

    friend inline std::ostream& operator << (std::ostream &s, Matrix<ROWS, COLS, TRANSPOSED, T> m);
};

#endif //MATRIX_H
