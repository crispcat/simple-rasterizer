#ifndef SCALAR_H
#define SCALAR_H

namespace scal
{
    template <class T>
    T clamp0(T val)
    {
        return val < 0 ? 0 : val;
    }

    template <class T>
    T clamp01(T val)
    {
        return val < 0 ? 0 : val > 1 ? 1 : val;
    }

    template <class T>
    T clamp(T val, T min, T max)
    {
        return val < min ? min : val > max ? max : val;
    }
}

#endif //SCALAR_H
