#include "matrix.h"

namespace matrix
{
    template<std::size_t N, typename T>
    __Matrix<N, N, false, T> matrix::identity()
    {
        __Matrix<N, N, false, T> id { };
        for (int i = 0; i < N; i++)
            id.set(i, i, static_cast<T>(1));
        return id;
    }
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
T __Matrix<ROWS, COLS, TRANSPOSED, T>::get(std::size_t r, std::size_t c)
{
  	if constexpr (TRANSPOSED)
  	{
    	return _el[c * COLS + r];
  	}
  	else
  	{
    	return _el[c * ROWS + r];
	}
};

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
void __Matrix<ROWS, COLS, TRANSPOSED, T>::set(std::size_t r, std::size_t c, T val)
{
  	if constexpr (TRANSPOSED)
  	{
    	_el[c * COLS + r] = val;
  	}
  	else
  	{
    	_el[c * ROWS + r] = val;
	}
};


template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
__Matrix<COLS, ROWS, !TRANSPOSED, T> __Matrix<ROWS, COLS, TRANSPOSED, T>::transpose()
{
    return __Matrix<COLS, ROWS, !TRANSPOSED, T> { _el };
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
void __Matrix<ROWS, COLS, TRANSPOSED, T>::operator += (const __Matrix& m)
{
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            this.set(i, j, this.get(i, j) + m.get(i, j));
    }
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
void __Matrix<ROWS, COLS, TRANSPOSED, T>::operator -= (const __Matrix& m)
{
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            this.set(i, j, this.get(i, j) - m.get(i, j));
    }
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
__Matrix<ROWS, COLS, false, T> __Matrix<ROWS, COLS, TRANSPOSED, T>::operator + (const __Matrix& m)
{
    __Matrix<ROWS, COLS, false, T> res { };
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            res.set(i, j, this.get(i, j) + m.get(i, j));
    }
    return res;
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
__Matrix<ROWS, COLS, false, T> __Matrix<ROWS, COLS, TRANSPOSED, T>::operator - (const __Matrix& m)
{
    __Matrix<ROWS, COLS, false, T> res { };
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
            res.set(i, j, this.get(i, j) - m.get(i, j));
    }
    return res;
}

template <std::size_t ROWS, std::size_t COLS, bool TRANSPOSED, typename T>
template <size_t R_COLS, bool R_TRANSPOSED>
__Matrix<ROWS, R_COLS, false, T> __Matrix<ROWS, COLS, TRANSPOSED, T>::operator * (const __Matrix<COLS, R_COLS, R_TRANSPOSED, T>& m)
{
    __Matrix<ROWS, R_COLS, false, T> res { };
    for (size_t j = 0; j < COLS; j++)
        for (size_t i = 0; i < ROWS; i++)
            for (size_t k = 0; k < COLS; k++)
                res.set(i, j, res.get(i, j) + this.get(i, k) * m.get(k, j));
    return res;
}

template<size_t ROWS, size_t COLS, bool TRANSPOSED, typename T>
std::ostream& operator << (std::ostream &s, __Matrix<ROWS, COLS, TRANSPOSED, T> m)
{
    for (size_t r = 0; r < ROWS; r++)
    {
        for (size_t c = 0; c < COLS; c++)
            s << m.get(r, c) << ' ';
        s << '\n';
    }
    return s;
}
