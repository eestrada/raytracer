#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <stdexcept>
#include <iostream>
#include <cmath>

namespace cg
{

namespace utils
{
    const static long double PI = std::acos(-1.0L);

    template <typename T>
    T clamp(T value, T min, T max)
    {
        if(value > max) return max;
        else if(value < min) return min;
        else return value;
    }

    template <typename T>
    T lerp(T a, T b, double bias)
    {
        return (a * (1.0 - bias)) + (b * bias);
    }

    template <typename T>
    T fit(T value, T oldmin, T oldmax, T newmin, T newmax)
    {
        double bias = clamp(double(value - oldmin)/double(oldmax - oldmin), 0.0, 1.0);

        return lerp(newmin, newmax, bias);
    }

    template <typename T>
    double radians(T degrees)
    {
        double retval = static_cast<double>(degrees) * (PI/180.0L);
        //std::cerr << degrees << " in degrees is " << retval << " in radians.\n";
        return retval;
    }

    template <typename T>
    double degrees(T radians)
    {
        double retval = static_cast<double>(radians) * (180.0L/PI);
        //std::cerr << radians << " in radians is " << retval << " in degrees.\n";
        return retval;
    }
} // end namespace utils

} // end namespace cg

#endif // end include guard

