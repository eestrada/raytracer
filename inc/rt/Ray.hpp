#if !defined(RAY_HPP)
#define RAY_HPP

#include <cstdint>
#include "cg/Vec3.hpp"
#include "cg/Vec4.hpp"

namespace rt
{

struct Ray
{
    cg::Vec3 pos, dir;
    int8_t depth;
};

struct RayHit
{
    cg::Clr4 clr;
    double distance;
    bool hit;
};

} // End namespace rt
#endif // end include guard
