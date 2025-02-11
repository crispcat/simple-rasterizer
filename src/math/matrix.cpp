#include "matrix.h"

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

template<> Hom Hom::embed(Vector3<float> v)
{
    return {{ v.x, v.y, v.z, 1.f }};
}

template<> Vec3 Hom::proj()
{
	return { x / t, y / t, z / t };
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
    {
        T sum = 0;
        for (size_t k = 0; k < COLS; k++)
            for (size_t r = 0; r < ROWS; r++)
                sum += this.get(i, k) * m.get(r, j);
        res.set(i, j, sum);
    }
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
