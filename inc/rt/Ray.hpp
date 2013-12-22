#if !defined(RAY_HPP)
#define RAY_HPP

#include <cstdint>
#include <memory>
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
    Ray data;
    cg::Clr4 clr;
    double distance;
};

typedef std::shared_ptr<RayHit> RayHit_ptr;
typedef std::shared_ptr<Ray> Ray_ptr;
} // End namespace "rt"

#endif // end include guard

