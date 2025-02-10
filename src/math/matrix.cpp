#include "matrix.h"

template <size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
inline T Matrix<ROWS, COLS, TRANSPOSED, T>::get(size_t r, size_t c)
{
  	if constexpr (TRANSPOSED)
  	{
    	return _el[c * COLS + r];
  	}
  	else
  	{
    	return _el[r * COLS + c];
	}
};

template <size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
inline void Matrix<ROWS, COLS, TRANSPOSED, T>::set(size_t r, size_t c, T val)
{
  	if constexpr (TRANSPOSED)
  	{
    	_el[c * COLS + r] = val;
  	}
  	else
  	{
    	_el[r * COLS + c] = val;
	}
};

using Hom = Matrix<4, 1, false, float>;
template<> Hom Hom::embed(Vector3<float> v)
{
    return {{ v.x, v.y, v.z, 1.f }};
}

template<> Vec3 Hom::proj()
{
	return { x / t, y / t, z / t };
}

template <size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
Matrix<ROWS, COLS, TRANSPOSED, T> operator += (Matrix<ROWS, COLS, TRANSPOSED, T> m1, const Matrix<ROWS, COLS, TRANSPOSED, T>& m2)
{
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            m1.set(i, j, m1.get(i, j) + m2.get(i, j));
    }
}

template <size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
Matrix<ROWS, COLS, TRANSPOSED, T> operator += (Matrix<ROWS, COLS, TRANSPOSED, T> m1, const Matrix<COLS, ROWS, !TRANSPOSED, T>& m2)
{
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            m1.set(i, j, m1.get(i, j) + m2.get(i, j));
    }
}

class Matrix
{




    void operator -= (Matrix<ROWS, COLS, T> m)
    {
        for (size_t i = 0; i < SIZE; i++)
            el[i] -= m.el[i];
    }

    Matrix<ROWS, COLS, T> operator + (Matrix<ROWS, COLS, T> m) const
    {
        Matrix<ROWS, COLS, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] + m.el[i];
        return res;
    }

    Matrix<ROWS, COLS, T> operator - (Matrix<ROWS, COLS, T> m) const
    {
        Matrix<ROWS, COLS, T> res;
        for (size_t i = 0; i < SIZE; i++)
            res[i] = el[i] - m.el[i];
        return res;
    }

    template<size_t L>
    Matrix<ROWS, L, T> operator * (Matrix<COLS, L, T> m) const
    {
        Matrix<ROWS, L, T> res;
        for (size_t i = 0; i < ROWS * L; i++)
            for (size_t j = 0; j < COLS; j++)
                res.el[i] += el[(i / L) * COLS + j] * m.el[j * L + (i % L)];
        return res;
    }

    static constexpr Matrix<ROWS, COLS, T> identity()
    {
        static_assert(ROWS == COLS, "identity matrix construction require a square matrix type");
        Matrix<ROWS, COLS, T> id;
        for (size_t i = 0; i < SIZE; i += ROWS + 1)
            id.el[i] = 1;
        return id;
    }

    template<typename>
    friend inline std::ostream& operator << (std::ostream &s, Matrix<ROWS, COLS, T> m);
};

template<size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
std::ostream& operator << (std::ostream &s, Matrix<ROWS, COLS, TRANSPOSED, T> m)
{
    for (size_t r = 0; r < ROWS; r++)
    {
        for (size_t c = 0; c < COLS; c++)
            s << m.el(r, c) << ' ';
        s << '\n';
    }
    return s;
}