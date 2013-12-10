#if !defined(RAY_HPP)
#define RAY_HPP

#include <cstdint>
#include "cg/Vec3.hpp"

namespace rt
{

struct Ray
{
    cg::Vec3 pos, dir;
    int8_t depth;
};

} // End namespace rt
#endif // end include guard
