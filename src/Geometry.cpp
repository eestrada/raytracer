#include <iostream>
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

rt::RayHit Sphere::trace(const rt::Ray &ray)
{
    throw err::not_implemented("rt::RayHit Sphere::trace(const rt::Ray &ray)");
    return rt::RayHit();
}

rt::RayHit Triangle::trace(const rt::Ray &ray)
{
    throw err::not_implemented("rt::RayHit Triangle::trace(const rt::Ray &ray)");
    return rt::RayHit();
}

} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::Triangle &t)
{
    // Add one to these for printing to obj format indexing
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::TriMesh &tm)
{
    return out;
}

